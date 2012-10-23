#include <stdio.h>

#include "CAccViewer.h"


CAccViewer*	CAccViewer::accViewer = 0;


CAccViewer::CAccViewer(void)
: CAccessory()
{
	accViewer	= this;
	viewer		= 0;
}


CAccViewer::~CAccViewer()
{
}


void CAccViewer::Init(void)
{
	CProjection::Init();

}


void CAccViewer::Precompute(void)
{
	CAccessory::Precompute();
}

