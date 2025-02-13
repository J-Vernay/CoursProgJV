#include <dino/dino_draw_utils.h>

DinoDrawCall Dino_CreateDrawCall_Circle(float radius, DinoColor color)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "monogram-bitmap.png";
    drawCall.vertices.reserve(6);
    DinoVec2 posA = {-radius, -radius};
    DinoVec2 posB = {radius, -radius};
    DinoVec2 posC = {-radius, radius};
    DinoVec2 posD = {radius, radius};
    drawCall.vertices.emplace_back(posA, 72, 0, color);
    drawCall.vertices.emplace_back(posB, 96, 0, color);
    drawCall.vertices.emplace_back(posC, 72, 24, color);
    drawCall.vertices.emplace_back(posB, 96, 0, color);
    drawCall.vertices.emplace_back(posC, 72, 24, color);
    drawCall.vertices.emplace_back(posD, 96, 24, color);
    return drawCall;
}

DinoDrawCall Dino_CreateDrawCall_Text(std::string_view text, DinoColor color, DinoColor colorBackground,
                                      DinoVec2* pOutSize)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "monogram-bitmap.png";
    // Il y a au maximum un quad = deux triangles pour chaque octet du texte, et un quad pour le fond.
    drawCall.vertices.reserve(6 + 6 * text.size());

    // On laisse les 6 premiers vertices pour le fond, pour qu'il soit dessiné en premier.
    // On donnera les positions à la fin, car on ne connait pas encore la taille du texte.
    if (colorBackground.a > 0)
        drawCall.vertices.resize(6);

    // Dimensions en pixels des lettres dans la texture. 
    constexpr uint16_t QUAD_WIDTH = 6;
    constexpr uint16_t QUAD_HEIGHT = 12;

    // Pour suivre la position des caractères au fur et à mesure.
    int32_t x = 0;
    int32_t y = 0;

    // Pour suivre la taille totale du texte.
    int32_t xMax = 0;

    for (unsigned char c : text) {
        if (c == '\n') {
            // Retour à la ligne
            xMax = std::max(xMax, x);
            x = 0;
            y += QUAD_HEIGHT;
        }
        else if (c <= 31) {
            // Caractères de contrôle, on ne les gère pas = on les ignore = on ne fait rien.
        }
        else {
            // Caractères étendus, en dehors de l'ASCII.
            // On ne les gère pas = on affiche un '?' en tant qu'erreur.
            if (c >= 128)
                c = '?';

            int32_t xIdx = c % 16;
            int32_t yIdx = c / 16;
            uint16_t u = static_cast<uint16_t>(xIdx * QUAD_WIDTH);
            uint16_t v = static_cast<uint16_t>(yIdx * QUAD_HEIGHT);

            DinoVec2 posA = {static_cast<float>(x), static_cast<float>(y)};
            DinoVec2 posB = {static_cast<float>(x + QUAD_WIDTH), static_cast<float>(y)};
            DinoVec2 posC = {static_cast<float>(x), static_cast<float>(y + QUAD_HEIGHT)};
            DinoVec2 posD = {static_cast<float>(x + QUAD_WIDTH), static_cast<float>(y + QUAD_HEIGHT)};
            drawCall.vertices.emplace_back(posA, u, v, color);
            drawCall.vertices.emplace_back(posB, u + QUAD_WIDTH, v, color);
            drawCall.vertices.emplace_back(posC, u, v + QUAD_HEIGHT, color);
            drawCall.vertices.emplace_back(posB, u + QUAD_WIDTH, v, color);
            drawCall.vertices.emplace_back(posC, u, v + QUAD_HEIGHT, color);
            drawCall.vertices.emplace_back(posD, u + QUAD_WIDTH, v + QUAD_HEIGHT, color);

            x += QUAD_WIDTH;
        }
    }
    xMax = std::max(xMax, x);

    float width = static_cast<float>(xMax);
    float height = static_cast<float>(y + QUAD_HEIGHT);

    if (colorBackground.a > 0) {
        // Les 6 premiers points ont été laissés libres pour le fond.
        drawCall.vertices[0] = {{0, 0}, 0, 0, colorBackground};
        drawCall.vertices[1] = {{width, 0}, 0, 0, colorBackground};
        drawCall.vertices[2] = {{0, height}, 0, 0, colorBackground};
        drawCall.vertices[3] = {{width, 0}, 0, 0, colorBackground};
        drawCall.vertices[4] = {{0, height}, 0, 0, colorBackground};
        drawCall.vertices[5] = {{width, height}, 0, 0, colorBackground};
    }
    if (pOutSize != nullptr)
        *pOutSize = {width, height};

    return drawCall;
}

DinoDrawCall Dino_CreateDrawCall_Polyline(std::span<DinoVec2 const> points, float width,
                                          DinoColor color)
{
    DinoDrawCall drawCall;

    // En bonus, pour plus d'infos sur l'algorithme, voir :
    // https://jvernay.fr/en/blog/points-triangulation/

    // On ne supporte pas les couleurs transparentes, désolé.
    if (color.a == 0)
        return drawCall; // Invisible.
    color.a = 255;

    if (points.size() <= 1)
        return drawCall; // Degenerate case: cannot trace a line from 0 or 1 point.

    // Only compute these constants once.
    float halfWidth = width / 2;

    size_t idxPoint = 0;

    // First point.
    DinoVec2 A = points[idxPoint];
    idxPoint += 1;

    DinoVec2 B;
    float lenAB = 0;
    while (idxPoint < points.size() && lenAB == 0) {
        // Get second point, such that A and B are distinct.
        B = points[idxPoint];
        lenAB = hypotf(B.x - A.x, B.y - A.y);
        idxPoint += 1;
    }
    if (lenAB == 0)
        return {}; // Degenerate case: all points are identical.

    for (; idxPoint <= points.size(); idxPoint += 1) {

        // Get next point C such that B and C are distinct.
        DinoVec2 C;
        float lenBC;
        if (idxPoint < points.size()) {
            C = points[idxPoint];
            lenBC = hypotf(C.x - B.x, C.y - B.y);
        }
        else {
            // [AB] is the last segment to render, no join is needed at the end.
            // Making C = A such that (AB) and (BC) are aligned, thus no join will be generated.
            C = A;
            lenBC = lenAB;
        }
        if (lenBC == 0)
            continue; // Empty segment, do nothing.

        // Compute quad for segment AB.

        float xAB = B.x - A.x, yAB = B.y - A.y;
        float xAA1 = halfWidth / lenAB * -yAB;
        float yAA1 = halfWidth / lenAB * xAB;
        DinoVec2 A1{A.x + xAA1, A.y + yAA1};
        DinoVec2 A2{A.x - xAA1, A.y - yAA1};
        DinoVec2 A1p{B.x + xAA1, B.y + yAA1};
        DinoVec2 A2p{B.x - xAA1, B.y - yAA1};

        // Encode quad for segment AB as two triangles.
        drawCall.vertices.emplace_back(A1, 0, 0, color);
        drawCall.vertices.emplace_back(A2, 0, 0, color);
        drawCall.vertices.emplace_back(A1p, 0, 0, color);
        drawCall.vertices.emplace_back(A2, 0, 0, color);
        drawCall.vertices.emplace_back(A1p, 0, 0, color);
        drawCall.vertices.emplace_back(A2p, 0, 0, color);

        // Determine the ABC angle's orientation.

        float xBC = C.x - B.x, yBC = C.y - B.y;
        float zAB_BC = xAB * yBC - yAB * xBC;

        // If zAB_BC == 0, A B C are aligned and no join is needed.
        if (zAB_BC != 0) {
            // Compute the endpoints of the next segment.

            float xBB1 = halfWidth / lenBC * -yBC;
            float yBB1 = halfWidth / lenBC * xBC;
            DinoVec2 B1{B.x + xBB1, B.y + yBB1};
            DinoVec2 B2{B.x - xBB1, B.y - yBB1};

            // Generate Bevel join triangle.
            if (zAB_BC < 0) {
                drawCall.vertices.emplace_back(B, 0, 0, color);
                drawCall.vertices.emplace_back(A1p, 0, 0, color);
                drawCall.vertices.emplace_back(B1, 0, 0, color);
            }
            else {
                drawCall.vertices.emplace_back(B, 0, 0, color);
                drawCall.vertices.emplace_back(A2p, 0, 0, color);
                drawCall.vertices.emplace_back(B2, 0, 0, color);
            }
        }
        // Prepare for next segment.
        A = B;
        B = C;
        lenAB = lenBC;
    }

    return drawCall;
}

DinoDrawCall Dino_CreateDrawCall_Sprite(std::string textureName, int w, int h, uint16_t u,
                                        uint16_t v, bool flip, DinoColor color)
{
    DinoDrawCall drawCall;
    drawCall.textureName = textureName;
    drawCall.vertices.reserve(6);

    float fW = static_cast<float>(w);
    float fH = static_cast<float>(h);
    DinoVec2 posA = {-fW, -fH};
    DinoVec2 posB = {fW, -fH};
    DinoVec2 posC = {-fW, fH};
    DinoVec2 posD = {fW, fH};

    if (flip) {
        drawCall.vertices.emplace_back(posA, u + w, v + 0, color);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0, color);
        drawCall.vertices.emplace_back(posC, u + w, v + h, color);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0, color);
        drawCall.vertices.emplace_back(posC, u + w, v + h, color);
        drawCall.vertices.emplace_back(posD, u + 0, v + h, color);
    }
    else {
        drawCall.vertices.emplace_back(posA, u + 0, v + 0, color);
        drawCall.vertices.emplace_back(posB, u + w, v + 0, color);
        drawCall.vertices.emplace_back(posC, u + 0, v + h, color);
        drawCall.vertices.emplace_back(posB, u + w, v + 0, color);
        drawCall.vertices.emplace_back(posC, u + 0, v + h, color);
        drawCall.vertices.emplace_back(posD, u + w, v + h, color);
    }

    return drawCall;
}