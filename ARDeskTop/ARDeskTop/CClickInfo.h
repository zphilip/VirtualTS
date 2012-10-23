#ifndef ___CClickInfo
#define ___CClickInfo

class CCursor;
class CMainCursor;
class CComponent3D;



class CClickInfo
{
public :
	enum
	{
		kFirst,
		kMiddle,
		kSecond,
		kLast
	};
public :
			double		clickPos[3];
			double		objectPos[3];
			double		vector[3];
			double		vector2[3];
			double		len;
			double		angle;
			double		normal[3];
			double		matrix[16];
public :
						CClickInfo(void);
						~CClickInfo();

			void		Click(CMainCursor* cursor, CComponent3D* object, int mode = kFirst);
			void		Click2(CCursor* cursor, CComponent3D* object, int mode = kFirst);
};


class CClickInfo3
{
public :
			CCursor*	cursor;
			CClickInfo	firstClickInfo;
			CClickInfo	currentClickInfo;
			double		d[3];
			double		dist;
public :
						CClickInfo3(void);

			void		Precompute(double v[3]);
};


void	MultiMatrix(double matrix[], double vector[], double vector2[]);
double	InnerProduct(double vector[], double vector2[]);
void	OuterProduct(double vector[], double vector2[], double out[]);


#endif
