/********************************************************************
    created:   2003/09/02
    filename:  gl2pdf.c
	
    author:    Micha Bieber	
*********************************************************************/

# if defined(_MSC_VER)
#    pragma warning(disable: 4244) // 'conversion' conversion from 'type1' to 'type2', possible loss of data
# endif


#include <time.h>
#include <float.h>

#include "gl2ps.h"
#include "gl2ps_p.h"

extern GL2PScontext *gl2ps;

/* The PDF */

enum GL2PDF_COMPRESSOR
{
	GL2PDF_NOCOMPRESSOR,
	GL2PDF_RL,
  GL2PDF_DEFLATE
} compressor;

enum GL2PDF_GROUPING
{
	GL2PDF_NOGROUPS,
	GL2PDF_GROUPS
} objectgrouping;

#define GL2PDF_FIXED_XREF_ENTRIES 7 
int cref[GL2PDF_FIXED_XREF_ENTRIES];
int streamlength;

typedef GL2PSvertex GL2PStriangle[3];
GL2PSlist *tlist, *ilist, *slist; /* triangles, images, strings */

/* In unfiltered mode the Shader stream in PDF requires to be in a 'big-endian' order */

size_t gl2pdfWriteBigEndian(unsigned long data, size_t bytes, FILE* fp)
{
	size_t i;
	size_t size = sizeof(unsigned long);
	for (i=1; i<=bytes; ++i)
	{
		fputc(0xff & (data >> (size-i) * 8) , fp);
	}
	return bytes;
}

int gl2pdfPrintCompressorType(enum GL2PDF_COMPRESSOR comp)
{
	if (comp == GL2PDF_DEFLATE)
	{	
		return fprintf(gl2ps->stream,
			"/Filter [/FlateDecode]\n");
	}	
	return 0;
}

/* Helper for pixmap and text lists */

GL2PSimage* gl2pdfCopyPixmap(GL2PSimage* im)
{
	int size;
	GL2PSimage* image = (GL2PSimage*)gl2psMalloc(sizeof(GL2PSimage));
 
  image->width = im->width;
  image->height = im->height;
  image->format = im->format;
  image->type = im->type;
  size = image->height*image->width*3*sizeof(GLfloat); /* FIXME: handle other types/formats */
  image->pixels = (GLfloat*)gl2psMalloc(size);
  memcpy(image->pixels, im->pixels, size);

	return image;
}

void gl2pdfFreePixmap(GL2PSimage* im)
{
	if(!im)
		return;
	if (im->pixels)
		gl2psFree(im->pixels);
	gl2psFree(im);
}

void gl2pdfAddPixmap2list(GL2PSimage* im)
{
	GL2PSimage* image = gl2pdfCopyPixmap(im);
 	gl2psListAdd(ilist, &image);		
}

void gl2pdfDeletePixmapList()
{
	int i;
	GL2PSimage** ptr;
	int size = gl2psListNbr(ilist);
	for (i=0; i<size; ++i)
	{
		ptr = gl2psListPointer(ilist,i);
		gl2pdfFreePixmap(*ptr);
	}
	gl2psListDelete(ilist);
}

GL2PSstring* gl2pdfCopyText(GL2PSstring* t)
{
  GL2PSstring* text = (GL2PSstring*)gl2psMalloc(sizeof(GL2PSstring));
  text->str = (char*)gl2psMalloc((strlen(t->str)+1)*sizeof(char));
  strcpy(text->str, t->str); 
  text->fontname = (char*)gl2psMalloc((strlen(t->fontname)+1)*sizeof(char));
  strcpy(text->fontname, t->fontname);
  text->fontsize = t->fontsize;
	text->alignment = t->alignment;
	
	return text;
}

void gl2pdfAddText2list(GL2PSstring* s)
{
	GL2PSstring* str = gl2pdfCopyText(s);
 	gl2psListAdd(slist, &str);		
}

void gl2pdfFreeText(GL2PSstring* text)
{
	if(!text)
		return;
	if (text->str)
		gl2psFree(text->str);
	if (text->fontname)
		gl2psFree(text->fontname);
	gl2psFree(text);
}

void gl2pdfDeleteTextList()
{
	int i;
	GL2PSstring** ptr;
	int size = gl2psListNbr(slist);
	for (i=0; i<size; ++i)
	{
		ptr = gl2psListPointer(slist,i);
		gl2pdfFreeText(*ptr);
	}
	gl2psListDelete(slist);
}

int gl2pdfPrintStrokeColor(GL2PSrgba rgba)
{
		return fprintf(gl2ps->stream, "%f %f %f RG\n", rgba[0], rgba[1], rgba[2]);
}

int gl2pdfPrintFillColor(GL2PSrgba rgba)
{
		return fprintf(gl2ps->stream, "%f %f %f rg\n", rgba[0], rgba[1], rgba[2]);
}


/* Print 1st PDF object - file info */

int gl2pdfPrintInfo()
{
  time_t now;
	struct tm *newtime;

	time(&now);
  newtime = gmtime( &now );

  return fprintf(gl2ps->stream, 
		"1 0 obj\n"
		"<<\n"
			"/Title (%s)\n"
			"/Creator (%s)\n"
			"/Producer (GL2PS %d.%d.%d, an OpenGL to PostScript Printing Library)\n"
			"/CreationDate (D:%d%02d%02d%02d%02d%02d)\n"
		">>\n"
		"endobj\n",
	  gl2ps->title, gl2ps->producer, GL2PS_MAJOR_VERSION, GL2PS_MINOR_VERSION, GL2PS_PATCH_VERSION,
	  newtime->tm_year+1900, 
		newtime->tm_mon+1, 
		newtime->tm_mday,
		newtime->tm_hour,
		newtime->tm_min,
		newtime->tm_sec
		);
}

/* Create catalog and page structure - 2nd and 3th PDF object */

int gl2pdfPrintCatalog()
{
	return fprintf(gl2ps->stream, 
		"2 0 obj\n"
		"<<\n" 
			"/Type /Catalog\n"
			"/Pages 3 0 R\n"
		">>\n"
		"endobj\n");
}

int gl2pdfPrintPages()		
{
	return fprintf(gl2ps->stream, 
		"3 0 obj\n"
		"<<\n" 
			"/Type /Pages\n"
			"/Kids [6 0 R]\n"
			"/Count 1\n"
		">>\n"
		"endobj\n");
}


/* Open stream for data - graphical objects, fonts etc. PDF object 4*/

int gl2pdfOpenDataStream()		
{
	int offs = 0;

	offs += fprintf(gl2ps->stream, 
		"4 0 obj\n"
		"<<\n" 
			"/Length 5 0 R\n" );
	offs += gl2pdfPrintCompressorType(compressor);
	offs += fprintf(gl2ps->stream, 
		">>\n"
		"stream\n");
	return offs;
}

/* Stream setup - Graphics state, fill background if allowed */

int gl2pdfOpenDataStreamWritePreface()		
{
	int offs;
  GLint index;
 	GLfloat rgba[4];
	
	offs = fprintf(gl2ps->stream, 
		"/GS1 gs\n");
  
	if(gl2ps->options & GL2PS_DRAW_BACKGROUND){
    if(gl2ps->colormode == GL_RGBA || gl2ps->colorsize == 0){
      glGetFloatv(GL_COLOR_CLEAR_VALUE, rgba);
    }
    else{
      glGetIntegerv(GL_INDEX_CLEAR_VALUE, &index);
      rgba[0] = gl2ps->colormap[index][0];
      rgba[1] = gl2ps->colormap[index][1];
      rgba[2] = gl2ps->colormap[index][2];
      rgba[3] = 0.;
    }
    offs += fprintf(gl2ps->stream,
	    "%f %f %f rg\n"
			"%d %d %d %d re\n",
	    rgba[0], rgba[1], rgba[2], 
	    gl2ps->viewport[0], gl2ps->viewport[1],
	    gl2ps->viewport[2], gl2ps->viewport[3] 
	    );
    offs += fprintf(gl2ps->stream,
			"f\n");  
	}
	return offs;
}

/* Use the functions above to create the first part of the PDF*/

void gl2pdfPrintHeader()
{
	int offs;

  tlist = gl2psListCreate(0, 1, sizeof(GL2PStriangle));
  ilist = gl2psListCreate(0, 1, sizeof(GL2PSimage));
  slist = gl2psListCreate(0, 1, sizeof(GL2PSstring));

	compressor = GL2PDF_NOCOMPRESSOR; 
	if (gl2ps->format == GL2PS_PDF_DEFLATED)
		compressor = GL2PDF_DEFLATE; 


	offs = fprintf(gl2ps->stream, "%%PDF-1.3\n");
	cref[0] = offs;

	offs += gl2pdfPrintInfo();   
	cref[1] = offs;
	
	offs += gl2pdfPrintCatalog();
	cref[2] = offs;  
	
	offs += gl2pdfPrintPages();
	cref[3] = offs;  

	offs += gl2pdfOpenDataStream();
	cref[4] = offs; /* finished in gl2pdfPrintFooter */
	streamlength = gl2pdfOpenDataStreamWritePreface();
}

/* The central primitive drawing */

void gl2pdfPrintPrimitive(void *a, void *b)
{
 	GL2PSprimitive *prim;
	GL2PStriangle t;

  prim = *(GL2PSprimitive**)a;

  if((gl2ps->options & GL2PS_OCCLUSION_CULL) && prim->culled) return;

  switch(prim->type){
  case GL2PS_PIXMAP :
		gl2pdfAddPixmap2list(prim->image);
		streamlength += fprintf(gl2ps->stream,
			"q\n"
			"%d 0 0 %d %f %f cm\n"
			"/Im%d Do\n"
			"Q\n"
			,prim->image->width, prim->image->height
			,prim->verts[0].xyz[0], prim->verts[0].xyz[1]
			,gl2psListNbr(ilist)-1);
    break;
  case GL2PS_TEXT :
		gl2pdfAddText2list(prim->text);
    streamlength += gl2pdfPrintFillColor(prim->verts[0].rgba);
    streamlength += fprintf(gl2ps->stream,			
			"BT\n"
			"/F%d %d Tf\n"
			"%f %f Td\n"
			"(%s) Tj\n"
			"ET\n"
	    ,gl2psListNbr(slist)-1
			,prim->text->fontsize, prim->verts[0].xyz[0], prim->verts[0].xyz[1],
			prim->text->str);
    break;
  case GL2PS_POINT :
    if(gl2ps->lastlinewidth != prim->width){
      gl2ps->lastlinewidth = prim->width;
      streamlength += fprintf(gl2ps->stream, "%f w\n", gl2ps->lastlinewidth);
		}
		streamlength += fprintf(gl2ps->stream, "1 J\n");
		streamlength += gl2pdfPrintStrokeColor(prim->verts[0].rgba);
    streamlength += fprintf(gl2ps->stream, "%f %f m\n%f %f l\nS\n",
	      prim->verts[0].xyz[0], prim->verts[0].xyz[1],
	      prim->verts[0].xyz[0], prim->verts[0].xyz[1]);
    streamlength += fprintf(gl2ps->stream, "0 J\n");
    break;
  case GL2PS_LINE :
    if(gl2ps->lastlinewidth != prim->width){
      gl2ps->lastlinewidth = prim->width;
      streamlength += fprintf(gl2ps->stream, "%f w\n", gl2ps->lastlinewidth);
		}
    if(prim->dash){
      streamlength += fprintf(gl2ps->stream, "[%d] 0 d\n", prim->dash);
		}
		streamlength += gl2pdfPrintStrokeColor(prim->verts[0].rgba);
    streamlength += fprintf(gl2ps->stream, "%f %f m\n%f %f l\nS\n",
	      prim->verts[0].xyz[0], prim->verts[0].xyz[1],
	      prim->verts[1].xyz[0], prim->verts[1].xyz[1]);
		if(prim->dash){
      streamlength += fprintf(gl2ps->stream, "[] 0 d\n", prim->dash);
		}
    break;
  case GL2PS_TRIANGLE :
		t[0] = prim->verts[0];
		t[1] = prim->verts[1];
		t[2] = prim->verts[2];
		gl2psListAdd(tlist, t);		
		streamlength += fprintf(gl2ps->stream,
			"BX /Sh%d sh EX\n", gl2psListNbr(tlist)-1);

/*TODO - perhaps more efficient for flat shading
		
		streamlength += gl2pdfPrintFillColor(prim->verts[0].rgba);
	  streamlength += fprintf(gl2ps->stream, "%f %f m\n%f %f l\n%f %f l\nf\n",
	      prim->verts[2].xyz[0], prim->verts[2].xyz[1],
	      prim->verts[1].xyz[0], prim->verts[1].xyz[1],
	      prim->verts[0].xyz[0], prim->verts[0].xyz[1]);
*/
    break;
  case GL2PS_QUADRANGLE :
    gl2psMsg(GL2PS_WARNING, "There should not be any quad left to print");
    break;
  default :
    gl2psMsg(GL2PS_ERROR, "Unknown type of primitive to print");
    break;
  }
}

/* close stream and ... */

int gl2pdfCloseDataStream()
{
	return fprintf(gl2ps->stream, 
		"endstream\n"
		"endobj\n");	
}

/* ... write the now known length object */

int gl2pdfPrintDataStreamLength(int l)
{
	return fprintf(gl2ps->stream,
		"5 0 obj\n"
		"%d\n"
		"endobj\n", l);
}

/* Create named shader objects */

int gl2pdfPrintShaderResources(int firstObject, int size)
{
	int offs = 0;
	int i;

	offs += fprintf(gl2ps->stream,
		"/Shading\n"
		"<<\n");
	
	for (i=0; i<size; ++i)
	{
		offs += fprintf(gl2ps->stream,
		"/Sh%d %d 0 R\n", i, firstObject+i);
	}
	offs += fprintf(gl2ps->stream,
		">>\n");

	return offs;
}

/* Create named pixmap objects */

int gl2pdfPrintPixmapResources(int firstObject, int size)
{
	int offs = 0;
	int i;

	offs += fprintf(gl2ps->stream,
		"/XObject\n"
		"<<\n");
	
	for (i=0; i<size; ++i)
	{
		offs += fprintf(gl2ps->stream,
		"/Im%d %d 0 R\n", i, firstObject + i);
	}
	offs += fprintf(gl2ps->stream,
		">>\n");

	return offs;
}

/* Create named font objects */

int gl2pdfPrintTextResources(int firstObject, int size)
{
	int offs = 0;
	int i;

	offs += fprintf(gl2ps->stream,
		"/Font\n"
		"<<\n");
	
	for (i=0; i<size; ++i)
	{
		offs += fprintf(gl2ps->stream,
		"/F%d %d 0 R\n", i, firstObject + i);
	}
	offs += fprintf(gl2ps->stream,
		">>\n");

	return offs;
}

/* Put the info created before in PDF objects */

int gl2pdfPrintSinglePage()		
{
  int offs;
	
	offs = fprintf(gl2ps->stream, 
		"6 0 obj\n"
		"<<\n" 
			"/Type /Page\n"
			"/Parent 3 0 R\n"
			"/MediaBox [%d %d %d %d]\n"
			"/Contents 4 0 R\n"
			"/Resources\n" 
				"<<\n" 
					"/ProcSet [/PDF /Text /ImageB /ImageC]  %%/ImageI\n"
					"/Length 5 0 R\n"
					"/ExtGState\n" 
						"<<\n"	
							"/GS1 7 0 R\n"
						">>\n",
								gl2ps->viewport[0], gl2ps->viewport[1], gl2ps->viewport[2], gl2ps->viewport[3]);
		
		offs += gl2pdfPrintShaderResources(GL2PDF_FIXED_XREF_ENTRIES + 1, gl2psListNbr(tlist));
		offs += gl2pdfPrintPixmapResources(GL2PDF_FIXED_XREF_ENTRIES + 1 
			                                          + gl2psListNbr(tlist), gl2psListNbr(ilist));
		offs += gl2pdfPrintTextResources(GL2PDF_FIXED_XREF_ENTRIES + 1 
			                                          + gl2psListNbr(tlist) 
																								+ gl2psListNbr(ilist), gl2psListNbr(slist));

		offs += fprintf(gl2ps->stream, 			
				">>\n"
		">>\n"
		"endobj\n");
	
		return offs;
}

/* Extended graphics state for shading */

int gl2pdfPrintExtGState()
{
	return fprintf(gl2ps->stream,
		"7 0 obj\n"
			"<<\n"
			"/Type /ExtGState\n"
			"/SA false\n"
			"/SM 0.02\n"
			"/OP false\n"
			"/op false\n"
			"/OPM 0\n"
			"/BG2 /Default\n"
			"/UCR2 /Default\n"
			"/TR2 /Default\n"
			">>\n"
		"endobj\n");
}


/* Put a triangles uncompressed raw data in shader stream */

int gl2pdfPrintShaderStreamData(GL2PSvertex* triangle)
{
  int offs = 0;
  int i;
	unsigned long imap;
	GLfloat  diff, dx, dy;
	
	char edgeflag = 0;
	double dmax = ~1UL;

	dx = gl2ps->viewport[2]-gl2ps->viewport[0];
	dy = gl2ps->viewport[3]-gl2ps->viewport[1];
	
	for (i = 0; i < 3; ++i)
  {
    offs += fwrite (&edgeflag, sizeof(char), 1, gl2ps->stream);
		
		if (fabs(dx*dy) < FLT_MIN)
		{
			offs += gl2pdfWriteBigEndian(0, 4, gl2ps->stream);
			offs += gl2pdfWriteBigEndian(0, 4, gl2ps->stream);
		}
		else
		{
			diff = (triangle[i].xyz[0] - gl2ps->viewport[0]) / dx;
			if (diff>1)
				diff = 1;
			else if (diff<0)
				diff = 0;
			imap = (unsigned long)(diff * dmax);
			offs += gl2pdfWriteBigEndian(imap, 4, gl2ps->stream);
			
			diff = (triangle[i].xyz[1] - gl2ps->viewport[1]) / dy;
			if (diff>1)
				diff = 1;
			else if (diff<0)
				diff = 0;
			imap = (unsigned long)(diff * dmax);
			offs += gl2pdfWriteBigEndian(imap, 4, gl2ps->stream);
		}
    
		imap = (unsigned long)(triangle[i].rgba[0] * dmax);
 		offs += gl2pdfWriteBigEndian(imap, 1, gl2ps->stream);
		
		imap = (unsigned long)(triangle[i].rgba[1] * dmax);
 		offs += gl2pdfWriteBigEndian(imap, 1, gl2ps->stream);
		
		imap = (unsigned long)(triangle[i].rgba[2] * dmax);
 		offs += gl2pdfWriteBigEndian(imap, 1, gl2ps->stream);
	}
	return offs;
}

/* Writes shaded triangle */

int gl2pdfPrintShader(int obj, GL2PSvertex* triangle )
{
	int offs = 0;
	int vertexbytes;

	offs += fprintf(gl2ps->stream,
		"%d 0 obj\n"
		"<<\n"
		"/ShadingType 4\n"
		"/ColorSpace /DeviceRGB\n"
		"/BitsPerCoordinate 32\n"
		"/BitsPerComponent 8\n"
		"/BitsPerFlag 8\n"
		"/Decode [%d %d %d %d 0 1 0 1 0 1]\n"
		,obj
		,gl2ps->viewport[0]
		,gl2ps->viewport[2]
		,gl2ps->viewport[1]
		,gl2ps->viewport[3]);
 	
	offs += gl2pdfPrintCompressorType(compressor);
	
	vertexbytes = 1+4+4+1+1+1;
	offs += fprintf(gl2ps->stream,
				"/Length %d\n"
				">>\n"
				"stream\n"
				,vertexbytes * 3
				);
	offs += gl2pdfPrintShaderStreamData(triangle);
	
	offs += fprintf(gl2ps->stream,
				"\nendstream\n"
				"endobj\n"
				);
	return offs;
}

/* Writes all triangles and returns field of offsets for the PDF cross reference table */

int* gl2pdfPrintShaderObjects(int firstObjnumber, int firstOffs)
{
	int size;
	int* offs;
	int i;

	size = gl2psListNbr(tlist);
	offs = (int*)malloc(sizeof(int) * (size+1));

	offs[0] = firstOffs;

	for (i=0; i<size; ++i)
	{
		firstOffs += gl2pdfPrintShader(i+firstObjnumber, (GL2PSvertex*)gl2psListPointer(tlist, i));
		offs[i+1] = firstOffs;
	}
	return offs;
}

/* Similar groups of  functions for pixmaps and text */

int gl2pdfPrintPixmapStreamData(GL2PSimage* im)
{
  int offs = 0;
	int x,y;
	GLfloat r,g,b;

	for (y=0; y<im->height;++y)
		for (x=0; x<im->width;++x)
		{
			gl2psGetRGB(im->pixels, im->width, im->height, x, y, &r, &g, &b);
			fputc((int)(r*255),gl2ps->stream);
			fputc((int)(g*255),gl2ps->stream);
			fputc((int)(b*255),gl2ps->stream);
		}
	return 3 * im->width * im->height;
}

int gl2pdfPrintPixmap(int obj, GL2PSimage* im )
{
	int offs = 0;

	offs += fprintf(gl2ps->stream,
		"%d 0 obj\n"
		"<<\n"
			"/Type /XObject\n"
			"/Subtype /Image\n"
			"/Width %d\n"
			"/Height %d\n"
			"/ColorSpace /DeviceRGB\n"
			"/BitsPerComponent 8\n"
			"/Length %d\n"
		">>\n"
		"stream\n"
		,obj, im->width, im->height, im->width * im->height * 3
		);

	offs += gl2pdfPrintPixmapStreamData(im);
	
	offs += fprintf(gl2ps->stream,
				"\nendstream\n"
				"endobj\n"
				);
	return offs;
}

int* gl2pdfPrintPixmapObjects(int firstObjnumber, int firstOffs)
{
	int size;
	int* offs;
	int i;

	size = gl2psListNbr(ilist);
	offs = (int*)malloc(sizeof(int) * (size+1));

	offs[0] = firstOffs;

	for (i=0; i<size; ++i)
	{
		firstOffs += gl2pdfPrintPixmap(i+firstObjnumber, *(GL2PSimage**)gl2psListPointer(ilist, i));
		offs[i+1] = firstOffs;
	}
	return offs;
}

int gl2pdfPrintText(int obj, GL2PSstring* s, int fontnumber )
{
	int offs = 0;

	offs += fprintf(gl2ps->stream,
		"%d 0 obj\n"
		"<<\n"
			"/Type /Font\n"
			"/Subtype /Type1\n"
			"/Name /F%d\n"
			"/BaseFont /%s\n"
			"/Encoding /MacRomanEncoding\n"
		">>\n"
		"endobj\n"
		,obj, fontnumber, s->fontname //TODO
		);
	return offs;
}

int* gl2pdfPrintTextObjects(int firstObjnumber, int firstOffs)
{
	int size;
	int* offs;
	int i;

	size = gl2psListNbr(slist);
	offs = (int*)malloc(sizeof(int) * (size+1));

	offs[0] = firstOffs;

	for (i=0; i<size; ++i)
	{
		firstOffs += gl2pdfPrintText(i+firstObjnumber, *(GL2PSstring**)gl2psListPointer(slist, i),i);
		offs[i+1] = firstOffs;
	}
	return offs;
}

/* All variable data is written at this point and all required function has been gathered: 
   Writes file footer with cross reference table and trailer */

void gl2pdfPrintFooter()
{
	int offs;
	int i;
	int *shader_offs, *image_offs, *text_offs;
	int shader_size, image_size, text_size, objnumber, lastoffset;

	offs = cref[4] + streamlength; 
	offs += gl2pdfCloseDataStream();
	cref[4] = offs; 

	offs += gl2pdfPrintDataStreamLength(streamlength);
	cref[5] = offs;
	streamlength = 0;

	offs += gl2pdfPrintSinglePage();
	cref[6] = offs;

	offs += gl2pdfPrintExtGState();

	shader_size = gl2psListNbr(tlist);
	image_size = gl2psListNbr(ilist);
	text_size = gl2psListNbr(slist);

	shader_offs = gl2pdfPrintShaderObjects(GL2PDF_FIXED_XREF_ENTRIES + 1, offs);
	image_offs = gl2pdfPrintPixmapObjects(GL2PDF_FIXED_XREF_ENTRIES + 1 + shader_size, shader_offs[shader_size]);
	text_offs = gl2pdfPrintTextObjects(GL2PDF_FIXED_XREF_ENTRIES + 1 + shader_size + image_size, image_offs[image_size]);

	lastoffset = text_offs[text_size];
	objnumber = GL2PDF_FIXED_XREF_ENTRIES + shader_size + image_size + text_size + 1;
	
	/* 
		Start cross reference table.
		The file has to been opened in binary mode to preserve 
		the 20 digit string length! 
	*/
	fprintf(gl2ps->stream,
		"xref\n"
		"0 %d\n"
		"%010d 65535 f \n", objnumber, 0); 
	
	for (i=0; i<GL2PDF_FIXED_XREF_ENTRIES; ++i)
	{
		fprintf(gl2ps->stream, "%010d 00000 n \n", cref[i]); 
	}
	for (i=0; i<shader_size; ++i)
	{
		fprintf(gl2ps->stream, "%010d 00000 n \n", shader_offs[i]); 
	}
	for (i=0; i<image_size; ++i)
	{
		fprintf(gl2ps->stream, "%010d 00000 n \n", image_offs[i]); 
	}
	for (i=0; i<text_size; ++i)
	{
		fprintf(gl2ps->stream, "%010d 00000 n \n", text_offs[i]); 
	}

	fprintf(gl2ps->stream,
		"trailer\n"
		"<<\n" 
			"/Size %d\n"
			"/Info 1 0 R\n"
			"/Root 2 0 R\n"
		">>\n"
		"startxref\n%d\n"
	  "%%%%EOF\n"
		, objnumber, lastoffset);

	/* Free auxiliary lists and arrays */
	free(shader_offs);
	free(image_offs);
	free (text_offs);
	gl2psListDelete(tlist);
	gl2pdfDeletePixmapList();
	gl2pdfDeleteTextList();
}

/* */

void gl2pdfPrintBeginViewport(GLint viewport[4])
{
	int offs;
  GLint index;
 	GLfloat rgba[4];
  int x = viewport[0], y = viewport[1], w = viewport[2], h = viewport[3];

	offs = 0;

  glRenderMode(GL_FEEDBACK);

  offs += fprintf(gl2ps->stream, 
	  "q\n");

	if(gl2ps->options & GL2PS_DRAW_BACKGROUND){
    if(gl2ps->colormode == GL_RGBA || gl2ps->colorsize == 0){
      glGetFloatv(GL_COLOR_CLEAR_VALUE, rgba);
    }
    else{
      glGetIntegerv(GL_INDEX_CLEAR_VALUE, &index);
      rgba[0] = gl2ps->colormap[index][0];
      rgba[1] = gl2ps->colormap[index][1];
      rgba[2] = gl2ps->colormap[index][2];
      rgba[3] = 0.;
    }
    offs += fprintf(gl2ps->stream,
	    "%f %f %f rg\n"
			"%d %d %d %d re\n"
//			"%d %d %d %d W\n"  
// This _should_ be correct PDF, nevertheless ghostscript can deal with this, unlike Acrobat.
// Looks like a Graphics state stack error. 
			"f\n"
	    ,rgba[0], rgba[1], rgba[2] 
	    ,x,y,w,h 
//	    ,gl2ps->viewport[0], gl2ps->viewport[1]
//	    ,gl2ps->viewport[2], gl2ps->viewport[3] 
	    );
	}
	streamlength += offs;
}

GLint gl2pdfPrintEndViewport(){
  GLint res;
  GLint gl2psPrintPrimitives(void);

  res = gl2psPrintPrimitives();
  streamlength += fprintf(gl2ps->stream, "Q\n");

  return res;
}