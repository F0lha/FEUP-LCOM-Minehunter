#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */

void vg_set_pixel(unsigned int x,unsigned int y, unsigned long color);
void vg_fill(unsigned int x,unsigned int y, unsigned int width, unsigned int height,unsigned long color);
void vg_set_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color);
static char *get_phys_mem();
#endif /* __VIDEO_GR_H */
