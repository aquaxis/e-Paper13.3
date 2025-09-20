#include <stdio.h>
#include <stdlib.h>

// 1200x1600ドットの画像用配列
unsigned char Image6color[960000];

// 4bitインデックスカラーのBitmapファイルをロードして配列に格納する関数
int loadbmp(const char *filename) {
    FILE *fp;
    unsigned char header[54];
    unsigned int dataOffset, width, height, bitsPerPixel;
    unsigned char *bitmapData;
    int ret = 1; // 戻り値。初期値はエラー

    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return ret;
    }

    // BMPヘッダーを読み込み
    if (fread(header, 1, 54, fp) != 54) {
        fprintf(stderr, "Error: Invalid BMP file header\n");
        fclose(fp);
        return ret;
    }

    // BMPファイルかどうかチェック
    if (header[0] != 'B' || header[1] != 'M') {
        fprintf(stderr, "Error: Not a BMP file\n");
        fclose(fp);
        return ret;
    }

    // ヘッダー情報を解析（エンディアン安全な方法）
    dataOffset = header[10] | (header[11] << 8) | (header[12] << 16) | (header[13] << 24);
    width = header[18] | (header[19] << 8) | (header[20] << 16) | (header[21] << 24);
    height = header[22] | (header[23] << 8) | (header[24] << 16) | (header[25] << 24);
    bitsPerPixel = header[28] | (header[29] << 8);

    // 要件の確認
    if (width != 1200 || height != 1600 || bitsPerPixel != 4) {
        fprintf(stderr, "Error: Image format mismatch. Expected 1200x1600, 4-bit indexed color.\n");
        fclose(fp);
        return ret;
    }

    // データ部分にシーク
    fseek(fp, dataOffset, SEEK_SET);

    // 4bitの場合、1行のバイト数は(width / 2)、ただし4バイト境界にアライン
    int rowBytes = ((width + 1) / 2 + 3) & ~3;
    int imageSize = rowBytes * height;

    bitmapData = (unsigned char*)malloc(imageSize);
    if (!bitmapData) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        fclose(fp);
        return ret;
    }

    // 画像データを読み込み
    if (fread(bitmapData, 1, imageSize, fp) != imageSize) {
        fprintf(stderr, "Error: Failed to read bitmap data\n");
        free(bitmapData);
        fclose(fp);
        return ret;
    }

    // BMPは下から上に格納されているため、上下反転して格納
    long long array_index = 0;
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x += 2) {
            int srcIndex = y * rowBytes + x / 2;
            Image6color[array_index++] = bitmapData[srcIndex];
        }
    }

    ret = 0; // 成功

    free(bitmapData);
    fclose(fp);

    return ret;
}
