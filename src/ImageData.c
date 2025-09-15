#include <stdio.h>
#include <stdlib.h>
#include <png.h>

// 1200x1600ドットの画像用配列
unsigned char Image6color[960000];

// PNGファイルをロードして配列に格納する関数
int loadpng(const char *filename) {
    png_structp png_ptr;
    png_infop info_ptr;
    FILE *fp;
    unsigned char **row_pointers;
    int ret = 1; // 戻り値。初期値はエラー

    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return ret;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return ret;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return ret;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return ret;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // 要件の確認
    if (width != 1200 || height != 1600 || color_type != PNG_COLOR_TYPE_PALETTE) {
        fprintf(stderr, "Error: Image format mismatch. Expected 1200x1600, indexed color.\n");
        goto cleanup;
    }

    // 8bit未満のインデックスカラーは8bitに拡張
    if (bit_depth < 8) {
        png_set_packing(png_ptr);
    }
    
    // 画像データを読み込むためのメモリを確保
    row_pointers = (unsigned char**) malloc(sizeof(unsigned char*) * height);
    if (!row_pointers) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        goto cleanup;
    }

    // png_get_rowbytes()はpng_set_packing()の影響を受ける
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (unsigned char*) malloc(png_get_rowbytes(png_ptr, info_ptr));
        if (!row_pointers[y]) {
            fprintf(stderr, "Error: Memory allocation failed for row %d.\n", y);
            for (int i = 0; i < y; i++) {
                free(row_pointers[i]);
            }
            free(row_pointers);
            goto cleanup;
        }
    }

    // 画像データを読み込む
    png_read_image(png_ptr, row_pointers);

    // 読み込んだ画像データを4bit/pixel形式に変換して格納
    long long array_index = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x += 2) {
            // 読み込んだインデックス値を取得
            // png_set_packing()により各インデックスは1バイトに拡張されている
            unsigned char pixel1_index = row_pointers[y][x];
            unsigned char pixel2_index = row_pointers[y][x + 1];

            // 1バイトに2つの4bitインデックスを格納
            Image6color[array_index++] = (pixel1_index << 4) | pixel2_index;
        }
    }

    ret = 0; // 成功

cleanup:
    if (row_pointers) {
        for (int y = 0; y < height; y++) {
            if (row_pointers[y]) {
                free(row_pointers[y]);
            }
        }
        free(row_pointers);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return ret;
}
