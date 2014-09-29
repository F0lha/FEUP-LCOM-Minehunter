#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {
	int i;
			char *ptr;
			ptr = video_mem;
			for(i = 0; i< scr_width*scr_lines; i++, ptr++) {
				*ptr = ch;
				ptr++;
				*ptr = attr;
			}

}

void vt_blank() {

	int i;
		char *ptr;
		ptr = video_mem;
		for(i = 0; i< scr_width*scr_lines; i++, ptr++) {
			*ptr = 0x00;
			ptr++;
			*ptr = 0x00;
		}

}

int vt_print_char(char ch, char attr, int r, int c) {
  char *ptr;
  if(r>scr_lines||c>scr_width) return 1;
  ptr = video_mem;
  ptr += scr_width*2*r;
  ptr += 2*c;
  ptr -= 2;
  *ptr = ch;
  ptr++;
  *ptr = attr;
  return 0;
}
  



int vt_print_string(char *str, char attr, int r, int c) {
	  char *ptr,*ptr_ori;
	  ptr = video_mem;
	  ptr_ori = video_mem;
	  ptr += scr_width*2*r;
	  ptr += 2*c;
	  ptr -= 2;
	  while(!(*str == '\0')){
		  *ptr = *str;
		  ptr++;
		  *ptr = attr;
		  ptr++;
		  str++;
	  }
	  if(ptr>(ptr_ori+scr_width*2*scr_lines)) return -1;
	  return 0;
}

int vt_print_int(int num, char attr, int r, int c) {
	char *ptr,*ptr_ori;
	int sinal = 0;
	ptr = video_mem + scr_width*2*scr_lines;
	ptr_ori = video_mem;
	ptr -= scr_width*2*scr_lines- scr_width*2*r;
	ptr -= scr_width*2-2*c;
	int i=0,num_cont =num;
	if(num < 0){
		num = num*(-1);
		num_cont = num_cont*(-1);
		sinal = 1;
	}
	while(num_cont>0){
		num_cont = num_cont / 10;
		i++;
	}
	ptr +=i*2;
	while(num > 0)
	{
		int cenas = num % 10;
		num = num / 10;
		ptr--;
		*ptr = attr;
		ptr--;
		*ptr = cenas+0x30;
	}
	if (sinal == 1){
		ptr--;
		*ptr = attr;
		ptr--;
		*ptr = '-';
	}
	if(ptr<ptr_ori) return -1;
	return 0;
}


int vt_draw_frame(int width, int height, char attr, int r, int c) {
	 char *ptr,*ptr_ori;
	 ptr = video_mem;
	 ptr_ori = video_mem;
	 if(width+c>scr_width||height+r>scr_lines) return -1;
	 ptr += scr_width*2*r;
	 ptr += 2*c;
	 ptr -= 1;
	 int i,diferenca=scr_width-(width+c);
	 for(i=0;i<=width;i++){
		 *ptr = attr;
		 ptr += 2;
	 }
	 ptr -= 2;
	 for(i=0;i<(height-2);i++){
		ptr+=diferenca*2;
		ptr+=c*2;
		*ptr = attr;
		ptr += width*2;
		*ptr = attr;
	 }
	 ptr+=diferenca*2 + c*2;
	 for(i=0;i<=width;i++){
	 		 *ptr = attr;
	 		 ptr += 2;
	 	 }
	 return 0;
}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
