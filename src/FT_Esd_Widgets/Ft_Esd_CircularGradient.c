
#include "Ft_Esd_Elements.h"
#include "Ft_Esd_Dl.h"
#include "Ft_Esd.h"
#include "Ft_Esd_Widget.h"
#include "Ft_Esd_CircularGradient.h"
#include "Ft_Esd_Primitives.h"

#define PIXELS(x) int((x)*16)
#define BITMAP_WIDTH 512
#define BITMAP_HEIGHT 1
#define BITMAP_TOTAL_SIZE (BITMAP_WIDTH * BITMAP_HEIGHT)

extern ESD_CORE_EXPORT EVE_HalContext *Esd_Host;
extern ESD_CORE_EXPORT Esd_GpuAlloc *Esd_GAlloc;

static uint8_t handle;
static uint32_t addr = GA_INVALID;

// Compressed data for pythag_raw_symmetric in decimal
static eve_progmem_const uint8_t pythag_raw_symmetric_compressed[] = {
	120,156,133,143,85,87,2,97,20,69,105,105,144,150,80,16,69,148,14,233,148,26,64,66,186,
	187,134,14,177,227,175,251,13,232,90,188,185,159,239,61,103,31,20,10,141,193,226,240,
	132,35,34,137,76,161,210,232,12,230,49,139,205,225,114,121,124,193,137,80,36,150,72,78,
	207,164,178,115,185,252,226,82,113,165,84,94,223,168,212,26,141,86,167,215,27,140,38,179,
	249,214,98,181,218,236,14,135,211,229,118,123,188,94,159,223,127,23,8,6,67,225,112,4,130,
	162,177,88,60,126,159,72,36,83,169,116,250,33,147,201,102,115,249,124,161,80,44,150,74,229,
	74,165,90,173,213,234,245,70,163,217,108,181,218,237,78,167,219,237,245,122,253,254,96,48,
	28,142,70,35,24,30,143,199,147,201,116,58,157,205,230,243,249,98,177,88,46,151,171,213,106,
	189,94,111,54,155,71,192,118,187,125,2,60,3,94,0,175,8,111,8,239,59,62,118,124,254,242,117,
	192,247,63,28,222,254,253,239,211,246,201,187,146,93,29,210,139,244,35,30,64,7,177,2,114,64,
	17,136,2,93,32,13,212,103,51,48,98,50,1,115,96,24,12,27,14,7,131,126,31,76,237,118,59,157,
	118,187,213,106,54,27,141,122,189,86,171,86,43,149,114,169,84,44,22,10,249,124,46,155,205,
	100,30,210,233,84,42,153,72,220,199,227,177,88,20,130,34,225,112,40,24,12,220,249,253,62,
	175,215,227,118,187,156,14,135,221,102,181,90,110,205,102,147,209,160,215,235,180,26,141,
	90,117,115,173,84,94,41,46,47,228,242,115,153,244,236,84,34,17,139,132,39,2,62,143,203,229,
	176,89,199,76,6,157,70,165,144,73,196,35,2,30,135,197,160,81,168,31,12,61,168,173,
};

// Compressed data for pythag_raw_symmetric in decimal
static eve_progmem_const uint8_t gauss_raw_symmetric_compressed[] = {
	120,156,93,141,103,59,130,1,24,70,127,41,82,68,169,20,162,168,140,66,34,43,169,140,22,17,
	161,68,40,132,16,81,34,37,73,91,123,111,217,111,248,194,249,248,92,207,125,78,69,101,21,8,
	84,13,134,64,106,106,161,208,186,122,24,12,222,128,64,34,81,141,104,12,166,169,185,5,139,
	109,109,195,225,241,237,29,4,34,137,212,217,213,221,67,38,83,122,251,250,169,212,1,218,224,
	16,157,62,60,50,58,54,206,96,76,48,39,89,108,14,103,106,122,102,150,203,227,241,5,194,185,
	121,145,104,97,81,188,180,44,145,172,172,174,73,101,178,117,249,198,166,66,177,181,189,163,
	84,169,118,247,246,213,234,131,195,35,141,230,248,228,84,171,61,59,215,233,46,46,245,122,195,
	149,209,120,125,99,50,221,154,205,150,59,171,245,222,102,123,176,219,31,29,14,167,203,229,
	118,123,188,94,159,239,201,239,15,4,130,161,80,56,28,137,68,163,177,88,60,158,72,36,147,169,
	84,58,157,206,100,178,217,92,46,151,207,231,11,133,98,177,248,12,80,42,149,94,0,94,1,222,202,
	188,255,240,241,151,207,207,127,135,223,183,239,73,121,91,118,0,170,178,17,16,23,10,64,2,8,101,
	179,153,12,144,77,165,146,201,68,34,30,143,197,162,209,72,36,28,14,133,130,129,128,223,255,228,
	243,121,189,30,183,219,229,114,58,28,143,118,251,131,205,118,111,181,222,89,204,230,91,147,233,
	230,218,104,188,50,232,245,151,23,58,221,249,153,86,123,122,114,172,209,28,29,30,168,213,251,123,
	187,42,149,114,103,123,75,161,216,220,144,175,203,100,210,181,213,21,137,100,121,73,188,184,32,
	18,205,207,9,5,124,30,143,59,59,51,61,197,225,176,89,147,204,9,6,99,124,108,116,100,152,78,31,26,
	164,13,80,169,253,125,189,20,50,185,167,187,171,147,68,34,18,58,218,241,120,92,91,43,22,219,210,
	220,132,193,160,27,81,72,36,162,1,14,131,213,215,65,161,181,53,16,8,184,26,4,170,170,172,248,2,
	92,7,123,150,0,
};

// clang-format on

static void CircularGradient_BitmapTransform(int dir, int w, int h)
{
	EVE_HalContext *phost = Esd_GetHost();

	if (dir == 0)
	{
		EVE_CoCmd_bitmapTransform(phost, 0, 0, w, 0, 0, h, 0, 0, 511, 0, 0, 511, 0);
	}
	else
	{
		EVE_CoCmd_bitmapTransform(phost, 0, 0, w, 0, 0, h, 0, 0, 0, 511, 511, 0, 0);
	}
}

uint32_t writeBitmapIntoGPUMemory(uint8_t gradientType)
{
	Ft_Esd_GpuHandle gpuHandle;

	EVE_HalContext *phost = Esd_GetHost();
	gpuHandle = Ft_Esd_GpuAlloc_Alloc(Ft_Esd_GAlloc, BITMAP_TOTAL_SIZE, GA_GC_FLAG);
	uint32_t addr = Ft_Esd_GpuAlloc_Get(Ft_Esd_GAlloc, gpuHandle);
	if (addr == GA_INVALID)
	{
		return GA_INVALID;
	}
	if (gradientType == ESD_PYTHAGOREAN)
		EVE_CoCmd_inflate_progMem(phost, addr, pythag_raw_symmetric_compressed, sizeof(pythag_raw_symmetric_compressed));
	else
		EVE_CoCmd_inflate_progMem(phost, addr, gauss_raw_symmetric_compressed, sizeof(gauss_raw_symmetric_compressed));
	return addr;
}

ESD_METHOD(Circular_Gradient_Widget_Render_Circular, Context = Ft_Esd_CircularGradient)
ESD_PARAMETER(x0, Type = int32_t, Default = 0)
ESD_PARAMETER(y0, Type = int32_t, Default = 0)
ESD_PARAMETER(x1, Type = int32_t, Default = 60)
ESD_PARAMETER(y1, Type = int32_t, Default = 20)
ESD_PARAMETER(color1, Type = ft_argb32_t, DisplayName = "outer color", Default = #FF3F3F3F)
ESD_PARAMETER(color2, Type = ft_argb32_t, DisplayName = "inner color", Default = #FFFFFFFF)
ESD_PARAMETER(gradientType, Type = Esd_CircularGradientType, DisplayName = "gradient type", Default = ESD_PYTHAGOREAN)
ft_void_t Circular_Gradient_Widget_Render_Circular(Ft_Esd_CircularGradient *context,
    int32_t x0, int32_t y0,
    int32_t x1, int32_t y1,
    ft_argb32_t color1, ft_argb32_t color2,
    uint8_t gradientType)
{
	if (addr == GA_INVALID)
		return;
	EVE_HalContext *phost = Esd_GetHost();
	
	int32_t w = x1 - x0;
	int32_t h = y1 - y0;
	int32_t X0 = x0 * 8;
	int32_t X1 = x1 * 8;
	int32_t Y0 = y0 * 8;
	int32_t Y1 = y1 * 8;
	
	EVE_CoDl_bitmapHandle(phost, handle);
#if (EVE_SUPPORT_CHIPID >= EVE_FT810)
	if (EVE_CHIPID >= EVE_FT810)
	{
		EVE_CoCmd_setBitmap(phost, addr, L8, BITMAP_WIDTH, BITMAP_HEIGHT);
	}
#else
	if (EVE_CHIPID < EVE_FT810)
	{
		EVE_CoDl_bitmapSource(phost, addr);
		EVE_CoDl_bitmapLayout(phost, L8, BITMAP_WIDTH, BITMAP_HEIGHT);
	}
#endif
	EVE_CoDl_bitmapSize(phost, BILINEAR, REPEAT, REPEAT, w, h);

	// The code below will do the circular gradient
	EVE_CoDl_saveContext(phost);
	EVE_CoDl_begin(phost, BITMAPS);

	Ft_Esd_Rect16 scissor = {
		.X = x0, .Y = y0, .Width = w, .Height = h
	};
	Esd_Scissor_Set(scissor); // No need to restore scissor afterwards, since context is saved

	EVE_CoDl_colorMask(phost, false, false, false, true);

	EVE_CoDl_vertexFormat(phost, 3);
	EVE_CoDl_bitmapHandle(phost, handle); // At any instant only one bitmap is loaded in memory.
	// Hence we use only one Bitmap Handle. By default it is starting with zero (0)

	EVE_CoDl_blendFunc(phost, ONE, ZERO); // Draw the X bitmap into alpha
	CircularGradient_BitmapTransform(0, w, h);
	EVE_CoDl_vertex2f(phost, X0, Y0);

	if (gradientType == ESD_PYTHAGOREAN)
		EVE_CoDl_blendFunc(phost, ONE, ONE); // For pythagorean, want alpha addition
	else
		EVE_CoDl_blendFunc(phost, DST_ALPHA, ZERO); // But for Gaussian, multiplication
	CircularGradient_BitmapTransform(1, w, h); // Blend the Y bitmap into alpha
	EVE_CoDl_vertex2f(phost, X0, Y0);

	EVE_CoDl_colorMask(phost, true, true, true, false); // Now use the alpha buffer
	EVE_CoDl_begin(phost, RECTS);

	EVE_CoDl_colorRgb_ex(phost, color1);
	EVE_CoDl_blendFunc(phost, ONE, ZERO);
	EVE_CoDl_vertex2f(phost, X0, Y0);
	EVE_CoDl_vertex2f(phost, X1, Y1);

	EVE_CoDl_colorRgb_ex(phost, color2);
	EVE_CoDl_blendFunc(phost, DST_ALPHA, ONE_MINUS_DST_ALPHA);
	EVE_CoDl_vertex2f(phost, X0, Y0);
	EVE_CoDl_vertex2f(phost, X1, Y1);
	EVE_CoDl_restoreContext(phost);
}
ESD_METHOD(Ft_Esd_CircularGradient_Update_Signal, Context = Ft_Esd_CircularGradient)
ESD_PARAMETER(gradientType, Type = Esd_CircularGradientType, DisplayName = "gradient type", Default = ESD_PYTHAGOREAN)
void Ft_Esd_CircularGradient_Update_Signal(Ft_Esd_CircularGradient *context, uint8_t gradientType)
{
	EVE_HalContext *phost = Esd_GetHost();
	handle = ESD_CO_SCRATCH_HANDLE;
	addr = writeBitmapIntoGPUMemory(gradientType);
}
