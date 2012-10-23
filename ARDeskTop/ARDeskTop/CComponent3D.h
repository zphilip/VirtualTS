#ifndef ___CComponent3D
#define ___CComponent3D

#include "CObject.h"
#include "CClickInfo.h"

class CGroup;
class CCursor;
class CMainCursor;
class CAnimeManager;
class CLink;



class CComponent3D : public CObject
{
public :
			double		matrix[16];
			double		matrixInv[3][4];
			bool		uniMatrix;

			float*		currentColor;
			float		color[4];

			double		width;
			double		height;
			double		depth;

			bool		touchable;
			bool		dropable;
			bool		changed;
			bool		fix;
			bool		drawBB;
			bool		notifyParent;
			bool		card;
			bool		flipZaxis;
			bool		fixedAxis[3];

			CGroup*		parent;
			CComponent3D*
						boundingBox;
			CCursor*	withinCursor;
			CMainCursor*
						clickedCursor;

			CAnimeManager*
						animeManager;
public :
						CComponent3D(void);
	virtual				~CComponent3D();

	virtual	CLink*		Remove(void);
	virtual	void		Delete(void);

	virtual	void		Precompute(void);
	virtual	void		Draw(void);
	virtual	void		DrawShape(void) = 0;
	virtual	void		KeyboardFunc3D(unsigned char key);
	virtual	CComponent3D*
						IsWithin(double x, double y, double z);
			CComponent3D*
						IsWithin(double pos[3]) { return(IsWithin(pos[0], pos[1], pos[2])); }
	virtual	void		DrawBoundingBox(void);

	virtual	void		Click(void);
	virtual	void		DoubleClick(void);
	virtual	void		InterruptClick(CMainCursor* cursor, int mode);
	virtual	void		InterruptTranslate(double* pos, double* objectPos);
	virtual	void		InterruptRotate(CMainCursor* cursor);

	virtual	void		CursorEnter(CCursor* cursor);
	virtual	void		CursorLeave(CCursor* cursor);
	virtual	void		CursorWithin(CCursor* cursor);
	virtual	void		ClickDown(CMainCursor* cursor);
	virtual	void		ClickUp(CMainCursor* cursor);
	virtual	void		DropFrom(CComponent3D* object, CMainCursor* cursor);
	virtual	void		DropTo(CComponent3D* object, CMainCursor* cursor);
	virtual	void		SetString(wchar_t* string);
	virtual	void		SetCardMode(bool pCard);

	virtual void		SetPosition(double x, double y, double z);
			void		SetPosition(double pos[3]) { SetPosition(pos[0], pos[1], pos[2]); }
			void		GetPosition(double pos[3]);

	virtual	void		SetSize(double w, double h, double d);
			void		GetSize(double size[3]);

			void		SetCurrentColor(float* pCurrentColor);
			void		SetColor3(float r, float g, float b);
			void		SetColor4(float r, float g, float b, float a);
			void		SetColor3v(float color[]) { SetColor3(color[0], color[1], color[2]); }
			void		SetColor4va(float color[], float a) { SetColor4(color[0], color[1], color[2], a); }
			void		SetColor4v(float color[]) { SetColor4(color[0], color[1], color[2], color[3]); }
			void		SetColor(void);
			void		SetAlpha(float a);
			float		GetAlpha(void);

			void		Scale(double x, double y, double z);
			void		Scale(double s) { Scale(s, s, s); }
			void		Rotate(double angle , double x, double y, double z);
			void		Multiply(double m[16]);

			void		SetParent(CGroup* pParent);
			void		GlobalToLocal(double pos[3]);
			void		LocalToGlobal(double pos[3]);
			void		GlobalToLocal(void);
			void		LocalToGlobal(void);
			void		GetGlobalMatrix(double mat[]);
			void		GetLocalMatrix(double mat[]);

			void		SetAnimeManager(CAnimeManager*);

			void		InvertPos(double x, double y, double z, double pos[3]);
			void		ConvertPos(double x, double y, double z, double pos[3]);

			void		FixAxis(bool x, bool y, bool z);

	virtual	void		Play(void);
	virtual	void		Replay(void);
	virtual	void		Stop(void);
	virtual	void		Pause(void);
	virtual	void		Skip(double skipTimeMS);
	virtual	void		Animate(void);

	// MMD—p
	virtual	void		UpdateMatrix(void) {}
	virtual	CComponent3D*
						GetHandleCube(void) { return(0); }

	friend	void		LoadIdentity(double matrix[16]);
	friend	void		ConvertToARMat(double matrix[16], double trans[3][4]);
	friend	void		CalcShadowMatrix(double* plane, double* lightPos, double* shadowMatrix);
};


#endif
