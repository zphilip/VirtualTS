#ifndef ___Color
#define ___Color

#define DEFAULT_ALPHA	0.5f


namespace Color {
	static	float	black[3]		= { 0.0f, 0.0f, 0.0f };
	static	float	white[3]		= { 1.0f, 1.0f, 1.0f };

	static	float	red[3]			= { 1.0f, 0.0f, 0.0f };
	static	float	green[3]		= { 0.0f, 1.0f, 0.0f };
	static	float	blue[3]			= { 0.0f, 0.0f, 1.0f };

	static	float	yellow[3]		= { 1.0f, 1.0f, 0.0f };
	static	float	cyan[3]			= { 0.0f, 1.0f, 1.0f };
	static	float	magenta[3]		= { 1.0f, 0.0f, 1.0f };

	static	float	pink[3]			= { 0xFF / 255.0f, 0x80 / 255.0f, 0x80 / 255.0f };
	static	float	orenge[3]		= { 0xFF / 255.0f, 0xCC / 255.0f, 0x00 / 255.0f };
	static	float	purple[3]		= { 0xC0 / 255.0f, 0x00 / 255.0f, 0xFF / 255.0f };

	static	float	truered[3]			= { 0xCC / 255.0f, 0x00 / 255.0f, 0x33 / 255.0f };
	static	float	elementalgreen[3]	= { 0x00 / 255.0f, 0xCC / 255.0f, 0x66 / 255.0f };
	static	float	marineblue[3]		= { 0x33 / 255.0f, 0xFF / 255.0f, 0xFC / 255.0f };
	static	float	nobleviolet[3]		= { 0x66 / 255.0f, 0x33 / 255.0f, 0xCC / 255.0f };
	static	float	madyellow[3]		= { 0x99 / 255.0f, 0x99 / 255.0f, 0x00 / 255.0f };
	static	float	passionorange[3]	= { 0xFF / 255.0f, 0x66 / 255.0f, 0x00 / 255.0f };

	inline	void	SetColor3(float c[], float r,  float g,  float b) { c[0] = r; c[1] = g; c[2] = b; }
	inline	void	SetColor3v(float c[], float d[]) { c[0] = d[0]; c[1] = d[1]; c[2] = d[2]; }
	inline	void	SetColor4(float c[], float r,  float g,  float b, float a) { c[0] = r; c[1] = g; c[2] = b; c[3] = a; }
	inline	void	SetColor4v(float c[], float d[]) { c[0] = d[0]; c[1] = d[1]; c[2] = d[2]; c[3] = d[3]; }
	inline	void	SetColor4va(float c[], float d[], float a) { c[0] = d[0]; c[1] = d[1]; c[2] = d[2]; c[3] = a; }
};


#endif
