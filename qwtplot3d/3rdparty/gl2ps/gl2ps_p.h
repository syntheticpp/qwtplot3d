#ifndef __gl2ps_p_2003_08_31_16_36__
#define __gl2ps_p_2003_08_31_16_36__

#ifdef __cplusplus
extern "C" {
#endif


void *gl2psMalloc(size_t size);
void *gl2psRealloc(void *ptr, size_t size);
void gl2psFree(void *ptr);
	
GLboolean gl2psSameColor(GL2PSrgba rgba1, GL2PSrgba rgba2);
GLboolean gl2psVertsSameColor(const GL2PSprimitive *prim);
void gl2psMsg(GLint level, char *fmt, ...);

void gl2psGetRGB(GLfloat *pixels, GLsizei width, GLsizei height, GLuint x, GLuint y, 
												GLfloat *red, GLfloat *green, GLfloat *blue);

GL2PSlist *gl2psListCreate(GLint n, GLint incr, GLint size);
void gl2psListDelete(GL2PSlist *list);
void gl2psListAdd(GL2PSlist *list, void *data);
GLint gl2psListNbr(GL2PSlist *list);
void *gl2psListPointer(GL2PSlist *list, GLint index);

void gl2pdfPrintPrimitive(void *a, void *b);
void gl2pdfPrintHeader();
void gl2pdfPrintFooter();
void gl2pdfPrintBeginViewport(GLint viewport[4]);
GLint gl2pdfPrintEndViewport();

#ifdef __cplusplus
};
#endif

#endif  // __gl2ps_p_2003_08_31_16_36__
