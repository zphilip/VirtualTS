#ifndef ___CObject
#define ___CObject



class CObject
{
public :
			int			id;
			bool		visible;
public :
						CObject(void);
	virtual				~CObject();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		Draw(void);
	virtual	void		KeyboardFunc3D(unsigned char key);

			void		SetID(int);
			int			GetID(void) { return(id); }
};


#endif
