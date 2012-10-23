//**********************
// テクスチャリスト管理
//**********************

#ifndef	_TEXTURELIST_H_
#define	_TEXTURELIST_H_


class cTextureList
{
	private :
		struct TextureData
		{
			unsigned int	uiTexID;
			unsigned int	uiRefCount;

			TextureData		*pNext;

			char			szFileName[32];
		};

		TextureData		*m_pTextureList;

		bool findTexture( const char *szFileName, unsigned int *puiTexID );

		bool createTexture( const char *szFileName, unsigned int *puiTexID );
		bool createFromBMP( const unsigned char *pData, unsigned int *puiTexID );
		bool createFromTGA( const unsigned char *pData, unsigned int *puiTexID );

	public :
		cTextureList( void );
		~cTextureList( void );

		unsigned int getTexture( const char *szDirFilePath, const char *szFileName ); //ちょっと変更

		void debugDraw( void );

		void releaseTexture( unsigned int uiTexID );
};

extern cTextureList		g_clsTextureList;

#endif	// _TEXTURELIST_H_
