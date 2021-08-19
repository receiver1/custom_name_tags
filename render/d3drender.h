/*

	PROJECT:		mod_sa
	LICENSE:		See LICENSE in the top level directory
	COPYRIGHT:		Copyright we_sux, BlastHack

	mod_sa is available from https://github.com/BlastHackNet/mod_s0beit_sa/

	mod_sa is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	mod_sa is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _D3DRENDER_H
#define _D3DRENDER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <cmath>
#include <string>

#include "../sdk/vector.hpp"

class	c_font;
class	c_render;

#define FCR_NONE	(1 << 1)
#define FCR_BOLD	(1 << 2)
#define FCR_ITALICS (1 << 3)
#define FCR_BORDER	(1 << 4)
#define FCR_SHADOW	(1 << 5)

//FVF Macros
#define D3DFVF_BITMAPFONT	( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define D3DFVF_PRIMITIVES	( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )

typedef struct d3dvertex_s
{
	float	x, y, z, rhw;
	DWORD	color;
	float	tu, tv;
} d3dvertex_s;

//Initialization Macros
#undef SAFE_RELEASE
#define SAFE_RELEASE( d ) \
	if ( d ) \
	{ \
		d->Release(); \
		d = NULL; \
	}

#define D3DFVF_XYZ			0x002
#define D3DFVF_RESERVED1	0x020
#define D3DFVF_DIFFUSE		0x040
#define D3DFVF_SPECULAR		0x080
#define D3DFVF_TEX1			0x100
#define D3DFVF_LVERTEX		( D3DFVF_XYZ | D3DFVF_RESERVED1 | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1 )

typedef float	D3DVALUE, *LPD3DVALUE;

typedef struct D3DLVERTEX
{
	union
	{
		D3DVALUE	x;			/* Homogeneous coordinates */
		D3DVALUE	dvX;
	};
	union
	{
		D3DVALUE	y;
		D3DVALUE	dvY;
	};
	union
	{
		D3DVALUE	z;
		D3DVALUE	dvZ;
	};
	DWORD	dwReserved;
	union
	{
		D3DCOLOR	color;		/* Vertex color */
		D3DCOLOR	dcColor;
	};
	union
	{
		D3DCOLOR	specular;	/* Specular component of vertex */
		D3DCOLOR	dcSpecular;
	};
	union
	{
		D3DVALUE	tu;			/* Texture coordinates */
		D3DVALUE	dvTU;
	};
	union
	{
		D3DVALUE	tv;
		D3DVALUE	dvTV;
	};
} D3DLVERTEX, *LPD3DLVERTEX;

class c_base_render
{
public:
	c_base_render();
	~c_base_render();

	static HRESULT	begin_render();
	static HRESULT	end_render();
protected:
	static HRESULT				Initialize(IDirect3DDevice9 *pD3Ddev);
	static HRESULT				Invalidate();

	static HRESULT				CreateStates();
	static HRESULT				DeleteStates();

	static IDirect3DDevice9 *m_pD3Ddev;
	static IDirect3DStateBlock9 *m_pD3DstateDraw;
	static IDirect3DStateBlock9 *m_pD3DstateNorm;

	static int					m_renderCount;
	static int					m_numShared;
	static bool					m_statesOK;
};

class c_font : public c_base_render
{
public:
	c_render *m_pRender;

	c_font(const char *szFontName, int fontHeight, DWORD dwCreateFlags);
	~c_font();

	HRESULT		initialize(IDirect3DDevice9 *pD3Ddev);
	HRESULT		invalidate();

	HRESULT		print(const char *text, D3DCOLOR color, float x, float y, bool skipColorTags, bool noColorFormat);
	HRESULT		print_shadow(float x, float y, DWORD color, const char *szText);

	float		draw_length(const char *szText, bool noColorFormat = false) const;
	size_t		get_char_pos(const char *text, float x, bool noColorFormat = false) const;

	float draw_height() const {
		return m_fChrHeight;
	};

	void set_font_name(const char *name) 
	{
		strcpy_s(m_szFontName, name);
		m_isReady = false;
	}

	void set_font_height(int height) {
		m_fontHeight = height;
	}

	void set_font_flags(std::uint32_t flags) {
		m_dwCreateFlags = flags;
	}

private:
	HRESULT					CreateVertexBuffers();
	HRESULT					DeleteVertexBuffers();

	char					m_szFontName[31 + 1];
	int						m_fontHeight;
	DWORD					m_dwCreateFlags;

	bool					m_isReady;

	IDirect3DTexture9 *m_pD3Dtex;
	IDirect3DVertexBuffer9 *m_pD3Dbuf;

	DWORD					m_maxTriangles;

	int						m_texWidth, m_texHeight;
	int						m_chrSpacing;
	float					m_fTexCoords[224][4];
	float					m_fChrHeight;
};

class c_render : public c_base_render
{
public:
	c_render(int numVertices);
	~c_render();

	HRESULT Initialize(IDirect3DDevice9 *pD3Ddev);
	HRESULT Invalidate();

	HRESULT Begin(D3DPRIMITIVETYPE primType);
	HRESULT End();

	HRESULT D3DColor(DWORD color);
	void	D3DBindTexture(IDirect3DTexture9 *);
	void	D3DTexCoord2f(float u, float v);
	HRESULT D3DVertex2f(s_vector2 pos);

	void	D3DTexQuad(s_vector2 shit, s_vector2 size, std::uint32_t color = -1, float su = 0.0f, float sv = 0.0f, float eu = 1.0f, float ev = 1.0f);
	void	D3DBox(s_vector2 pos, s_vector2 size, std::uint32_t color);
	void	border_box(s_vector2 pos, s_vector2 size, std::uint32_t border_color, std::uint32_t color);
	void	D3DLine(s_vector2 begin, s_vector2 end, D3DCOLOR color);
	bool	DrawLine(const D3DXVECTOR3 &a, const D3DXVECTOR3 &b, DWORD dwColor);
private:
	D3DPRIMITIVETYPE		m_primType;
	IDirect3DVertexBuffer9 *m_pD3Dbuf;
	d3dvertex_s *m_pVertex;

	DWORD m_color;
	float m_tu, m_tv;
	IDirect3DTexture9 *m_texture;
	int	m_maxVertex;
	int	m_curVertex;

	bool m_canRender;
};
#endif
