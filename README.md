# md5

嵌入式 md5 算法， 小端。在 ARM® Cortex™-M4 上测试通过。

## 特点

- 基于 mbedtls 的实现修改， 不依赖任何外部文件，包括宏定义和函数引用。

## 应用举例

- 生成 UUIDv3

```c

#include "md5.h"

// #define UUIDV3 (1)
#ifdef UUIDV3
// 使用这个随机的 uuidv4 作为卡片的名称 uuidv3 的命名空间
// #define CARD_UUID_NAMESPACE "45b879d0-80e2-4db5-a870-560e0fbe3992"
#define MD5_DIGEST_LENGTH (16)

// 使用 MD5 算法生成 UUIDv3， 用于卡片的名称， 确保同一张卡片在不同的设备上生成的名称uuid是一样的
// 命名空间为  "45b879d0-80e2-4db5-a870-560e0fbe3992"， 名称为卡片的 ID
void generate_uuid3(uint8_t *uuid) {

    // "45b879d0-80e2-4db5-a870-560e0fbe3992"
    unsigned char namespace_id[16] = {0x45, 0xb8, 0x79, 0xd0, 0x80, 0xe2, 0x4d, 0xb5, 0xa8, 0x70, 0x56, 0x0e, 0x0f, 0xbe, 0x39, 0x92};

    unsigned char input_data[16 + 16] = {0};  // 命名空间 + 名称
    memcpy(input_data, namespace_id, 16);  // 复制命名空间
    memcpy(input_data + 16, app_nfc_db_env.last_card_note.card_id, app_nfc_db_env.last_card_note.card_id_lg);  // 追加 ID
    log_info_array_ex("[nfc][generate_uuid3] input_data", input_data, 16 + 16);

    // 计算 MD5
    mbedtls_md5(input_data, 16 + app_nfc_db_env.last_card_note.card_id_lg, uuid);
    log_info_array_ex("[nfc][generate_uuid3] md5", uuid, MD5_DIGEST_LENGTH);

    // 设置 UUID 版本为 3
    uuid[6] = (uuid[6] & 0x0F) | 0x30;
    // 设置 UUID 变体为 10xx
    uuid[8] = (uuid[8] & 0x3F) | 0x80;
    log_info_array_ex("[nfc][generate_uuid3] name uuid", uuid, MD5_DIGEST_LENGTH);
}
#endif  // UUIDV3

```
