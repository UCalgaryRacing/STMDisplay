
/*******************************************************************************
* image
* filename: unsaved
* name: test
*
* preset name: Color R5G6B5
* data block size: 16 bit(s), uint16_t
* RLE compression enabled: no
* conversion type: Color, not_used not_used
* bits per pixel: 16
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: no
*******************************************************************************/

#include <stdint.h>



 typedef struct {
     const uint16_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;


static const uint16_t image_data_test[16] = {
    0xffff, 0xff00, 0xff00, 0xf0ff, 
    0xffff, 0xf0f0, 0xf0f0, 0xf0ff, 
    0xffff, 0xf00f, 0xf00f, 0xf0ff, 
    0xffff, 0xff0f, 0xff0f, 0xf0ff
};
const tImage test = { image_data_test, 4, 4,
    16 };

