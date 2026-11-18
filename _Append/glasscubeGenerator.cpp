#pragma once
#include "pch.h"   // 既存プロジェクトが pch.h の場合

#include "stdafx.h"
#include "GlassCubeGenerator.h"

void CreateGlassCubeTkmFile()
{
    // アセットフォルダを作成確認
    std::string modelPath = "Assets/modelData/";
    CreateDirectoryA(modelPath.c_str(), NULL);

    // 出力ファイルパス
    std::string outputPath = "Assets/modelData/GlassCube.tkm";
    FILE* fp = fopen(outputPath.c_str(), "wb");
    if (fp == nullptr) {
        MessageBoxA(nullptr, "ファイルの作成に失敗しました。", "エラー", MB_OK);
        return;
    }

    // ヘッダの作成
    uint8_t version = 100;
    uint8_t isFlatShading = 0;
    uint16_t numMeshParts = 1;
    fwrite(&version, sizeof(version), 1, fp);
    fwrite(&isFlatShading, sizeof(isFlatShading), 1, fp);
    fwrite(&numMeshParts, sizeof(numMeshParts), 1, fp);

    // メッシュパーツヘッダ
    uint32_t numMaterial = 1;
    uint32_t numVertex = 24;  // 立方体: 6面 * 4頂点
    uint8_t indexSize = 4;    // 32ビットインデックス
    uint8_t pad[3] = { 0, 0, 0 };
    fwrite(&numMaterial, sizeof(numMaterial), 1, fp);
    fwrite(&numVertex, sizeof(numVertex), 1, fp);
    fwrite(&indexSize, sizeof(indexSize), 1, fp);
    fwrite(pad, sizeof(pad), 1, fp);

    // マテリアル情報
    auto writeTextureName = [fp](const std::string& name) {
        uint32_t len = static_cast<uint32_t>(name.length());
        fwrite(&len, sizeof(len), 1, fp);
        if (len > 0) {
            fwrite(name.c_str(), len + 1, 1, fp);
        }
    };

    // 硬コードされたマップデータのファイル名
    std::string albedoMapName = "test.DDS";
    std::string normalMapName = "";
    std::string specularMapName = "spec.DDS";
    std::string reflectionMapName = "";
    std::string refractionMapName = "";

    writeTextureName(albedoMapName);
    writeTextureName(normalMapName);
    writeTextureName(specularMapName);
    writeTextureName(reflectionMapName);
    writeTextureName(refractionMapName);

    // 立方体の頂点データ
    // 1辺の長さが100.0で、中心はcenterで指定した座標
    float halfSize = 20.0f;
    // 立方体の中心点（ここを変更することで立方体の生成位置をずらせる）
    Vector3 center = { 50.0f, 25.0f, 50.0f };

    // 頂点データ構造: pos[3], normal[3], uv[2], weights[4], indices[4]
    struct VertexData {
        float pos[3];
        float normal[3];
        float uv[2];
        float weights[4];
        int16_t indices[4];
    };

    std::vector<VertexData> vertices;
    // 立方体の6面を定義
    // +X面 (右)
    vertices.push_back({ {halfSize, -halfSize, -halfSize}, {1,0,0}, {0,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {halfSize,  halfSize, -halfSize}, {1,0,0}, {1,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {halfSize,  halfSize,  halfSize}, {1,0,0}, {1,1}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {halfSize, -halfSize,  halfSize}, {1,0,0}, {0,1}, {1,0,0,0}, {-1,-1,-1,-1} });

    // -X面 (左)
    vertices.push_back({ {-halfSize, -halfSize,  halfSize}, {-1,0,0}, {0,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {-halfSize,  halfSize,  halfSize}, {-1,0,0}, {1,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {-halfSize,  halfSize, -halfSize}, {-1,0,0}, {1,1}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {-halfSize, -halfSize, -halfSize}, {-1,0,0}, {0,1}, {1,0,0,0}, {-1,-1,-1,-1} });

    // +Y面 (上)
    vertices.push_back({ {-halfSize,  halfSize, -halfSize}, {0,1,0}, {0,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ { halfSize,  halfSize, -halfSize}, {0,1,0}, {1,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ { halfSize,  halfSize,  halfSize}, {0,1,0}, {1,1}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {-halfSize,  halfSize,  halfSize}, {0,1,0}, {0,1}, {1,0,0,0}, {-1,-1,-1,-1} });

    // -Y面 (下)
    vertices.push_back({ {-halfSize, -halfSize,  halfSize}, {0,-1,0}, {0,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ { halfSize, -halfSize,  halfSize}, {0,-1,0}, {1,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ { halfSize, -halfSize, -halfSize}, {0,-1,0}, {1,1}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {-halfSize, -halfSize, -halfSize}, {0,-1,0}, {0,1}, {1,0,0,0}, {-1,-1,-1,-1} });

    // +Z面 (前)
    vertices.push_back({ {-halfSize, -halfSize,  halfSize}, {0,0,1}, {0,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {-halfSize,  halfSize,  halfSize}, {0,0,1}, {1,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ { halfSize,  halfSize,  halfSize}, {0,0,1}, {1,1}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ { halfSize, -halfSize,  halfSize}, {0,0,1}, {0,1}, {1,0,0,0}, {-1,-1,-1,-1} });

    // -Z面 (後ろ)
    vertices.push_back({ { halfSize, -halfSize, -halfSize}, {0,0,-1}, {0,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ { halfSize,  halfSize, -halfSize}, {0,0,-1}, {1,0}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {-halfSize,  halfSize, -halfSize}, {0,0,-1}, {1,1}, {1,0,0,0}, {-1,-1,-1,-1} });
    vertices.push_back({ {-halfSize, -halfSize, -halfSize}, {0,0,-1}, {0,1}, {1,0,0,0}, {-1,-1,-1,-1} });

    // 頂点データを書き込み（中心点分のオフセットを加算する）
    for (auto vertex : vertices) {
        vertex.pos[0] += center.x;
        vertex.pos[1] += center.z;
        vertex.pos[2] += center.y;
        fwrite(&vertex, sizeof(VertexData), 1, fp);
    }

    // インデックスデータ (ポリゴン数 = 12, インデクス数 = 36)
    int numPolygon = 12;
    fwrite(&numPolygon, sizeof(numPolygon), 1, fp);

    // 立方体のインデックス (3ds Max では1から開始するので、+1してから書き込む)
    // 注意: 四角形は必ず同じ対角線(n〜n+2)で2つの三角形に分割する。
    //       +X,-X面は頂点の巻き順が法線と一致しているためそのままだが、
    //       +Y,-Y,+Z,-Z面は頂点の巻き順が法線と逆になっているため、三角形の頂点順序を反転させている。
    uint32_t indices[] = {
        // +X面
        0,1,2,  0,2,3,
        // -X面
        4,5,6,  4,6,7,
        // +Y面（巻き順反転）
        8,10,9,  8,11,10,
        // -Y面（巻き順反転）
        12,14,13,  12,15,14,
        // +Z面（巻き順反転）
        16,18,17,  16,19,18,
        // -Z面（巻き順反転）
        20,22,21,  20,23,22
    };

    // TkmFile::LoadIndexBuffer は読み込み時に「index - 1」を行う（3ds Max形式が1始まりのため）。
    // このため、0始まりで定義したインデックスは+1してから書き込む必要がある。
    for (uint32_t idx : indices) {
        uint32_t writeIdx = idx + 1;
        fwrite(&writeIdx, sizeof(uint32_t), 1, fp);
    }

    fclose(fp);
    MessageBoxA(nullptr, "GlassCube.tkm ファイルを作成しました。", "成功", MB_OK);
}