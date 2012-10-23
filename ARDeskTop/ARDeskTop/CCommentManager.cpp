#include <stdio.h>
#include <locale.h>
#include <time.h>

#include "CCommentManager.h"
#include "CCVPictureFrame.h"
#include "CAnimeLiner.h"
#include "CAnimeMatrix.h"
#include "CARDeskTop.h"
#include "CGroup.h"
#include "CDesktop.h"
#include "CWorld.h"



void CCommentLine::Init(void)
{
	reachTime	= 0.0f;
	endTime		= 0.0f;
	height		= 0.0f;
}


void CCommentLayer::Init(void)
{
	for(int i = 0; i < LINE_NUM; i++)
		line[i].Init();
}


float CCommentLayer::AddComment(CComment* comment, CCommentData* data, float reachTime, float endTime, float max)
{
	float	heightTotal = 0.0;

	for(int i = 0; i < LINE_NUM; i++) {
		if(heightTotal + CComment::sizeList[data->size] > max) break;
		if(line[i].endTime < reachTime) {
			line[i].endTime	= endTime;
			line[i].height	= CComment::sizeList[data->size];

			if(data->layer == CComment::kTwist)
				heightTotal += CComment::sizeList[data->size] * 0.5;
			
			return(heightTotal);
		}

		heightTotal += line[i].height;
	}

	srand(data->id);

	heightTotal = rand() % (int) (max - CComment::sizeList[data->size]);
	if(data->layer == CComment::kTwist)
		heightTotal += CComment::sizeList[data->size] * 0.5;
	
	return(heightTotal);
}




CCommentManager::CCommentManager(CComponent3D* pComponent)
: CAnimeManager(pComponent)
{
	commentData	= 0;
	commentNum	= 0;
}


CCommentManager::~CCommentManager()
{
	CleanUp();
}


void CCommentManager::Init(void)
{
	CAnimeManager::Init();
}


int CCommentManager::Animate(void)
{
	if(!CAnimeManager::Animate()) return(0);

	float			pos, reachTime, animetionTime;
	double			commentPos[3], vec, height;
	int				layer;
	CCVPictureFrame*	video;
	wchar_t			str[256];

	animetionTime	= 3.0f;
	video			= (CCVPictureFrame*) component;
	height			= video->width * video->picture->height2 / video->picture->width2;

	for(int i = 0; i < commentNum; i++) {
		if(!commentData[i]) continue;
		if((float) commentData[i]->time > elapsedTimeMS) continue;

		layer = commentData[i]->layer;
		printf("add\n");

		if(layer == CComment::kPict) {
			setlocale(LC_ALL, "Japanese");
			mbstowcs(str, commentData[i]->str, strlen(commentData[i]->str) + 1);
			video->OpenFile(str);
		} else if(layer == CComment::kSound) {
			setlocale(LC_ALL, "Japanese");
			mbstowcs(str, commentData[i]->str, strlen(commentData[i]->str) + 1);
			ARDTPlaySound(str);
		} else {
			CComment* comment = new CComment();

			comment->Init();
			comment->SetMarginH(0.0);
			comment->SetMarginV(0.0);
			video->AddObject(comment);

			comment->SetCommentColor(commentData[i]->color);		//色
			comment->SetCommentSize(commentData[i]->size);			//フォントサイズ
			comment->SetCommentAlignment(commentData[i]->layer);	//アレンジ

			//コメント追加
			comment->SetString(commentData[i]->str);
			comment->Precompute();
			if(layer == CComment::kNaka || layer == CComment::kTate) {
				reachTime = commentData[i]->time + animetionTime * video->width / (video->width + comment->width3);
			} else {
				reachTime = commentData[i]->time + animetionTime;
			}

			if(layer == CComment::kTate) {
				pos = layerList[layer].AddComment(comment, commentData[i], reachTime, commentData[i]->time + animetionTime, video->width);
			} else {
				pos = layerList[layer].AddComment(comment, commentData[i], reachTime, commentData[i]->time + animetionTime, height);
			}

			vec = 0.0;

			//コメント位置
			switch(layer) {
			case CComment::kNaka :
			case CComment::kTwist :
				vec = -(video->width + comment->width3) / animetionTime;
				commentPos[0] = video->width * 0.5 - vec * (commentData[i]->time - elapsedTimeMS);
				commentPos[1] = height * 0.5 - pos;
				break;
			case CComment::kUe :
				commentPos[0] = 0.0;
				commentPos[1] = height * 0.5 - pos;
				break;
			case CComment::kShita :
				commentPos[0] = 0.0;
				commentPos[1] = -height * 0.5 + pos;
				break;
			case CComment::kHidari :
				commentPos[0] = -video->width * 0.5;
				commentPos[1] = -height * 0.5 + pos;
				break;
			case CComment::kMigi :
				commentPos[0] = video->width * 0.5;
				commentPos[1] = height * 0.5 - pos;
				break;
			case CComment::kTate :
				vec = -(video->width + comment->width3) / animetionTime;
				commentPos[0] = -video->width * 0.5 + pos;
				commentPos[1] = height * 0.5 - vec * (commentData[i]->time - elapsedTimeMS);
				break;
			}

			if(commentData[i]->depth == 0) { //オート
				commentPos[2] = comment->width3 + 5.0;
			} else { //手動
				commentPos[2] = commentData[i]->depth;
			}

			comment->SetPosition(commentPos);
			comment->SetCommentAnime(commentData[i]->layer, animetionTime, vec);	//アニメーション
		}

		delete commentData[i];
		commentData[i] = 0;
	}

	return(1);
}


void CCommentManager::Read(char* fileName)
{
	FILE*	fp;
	char	buf[512], layerName[64], size[64], color[64];
	int		i, m, s, ms;

	fp = fopen(fileName, "r");
	if(!fp) {
		printf("Can't find the file\n");
		return;
	}

	GetBuff(buf, 512, fp);
	sscanf(buf, "%d,%d:%d:%d,", &commentNum, &m, &s, &ms);

	if(component->animeManager) {
		animetionTimeMS = component->animeManager->animetionTimeMS;
	} else {
		animetionTimeMS = m * 60.0f + s + ms / 60.0f;
	}


	commentData = new CCommentData*[commentNum];

	for(i = 0; i < commentNum; i++) {
		if(!GetBuff(buf, 256, fp)) break;
		commentData[i] = new CCommentData();
		sscanf(buf, "%d,%d:%d:%d,%[^,],%[^,],%[^,],%d,%[^\n]",
			&(commentData[i]->id), &m, &s, &ms, layerName, size, color, &(commentData[i]->depth), commentData[i]->str);

	//	ms = i * 5 % 60; //00なので
		commentData[i]->time = m * 60.0f + s + ms / 60.0f;

		//レイヤー
		commentData[i]->SetLayer(layerName);
		//サイズ
		commentData[i]->SetSize(size);
		//色
		commentData[i]->SetColor(color);
	}

	for(i = 0; i < LAYER_NUM; i++)
		layerList[i].Init();
}


void CCommentManager::CleanUp(void)
{
	if(!commentData) return;

	for(int i = 0; i < commentNum; i++) {
		if(commentData[i]) {
			delete commentData[i];
			commentData[i] = 0;
		}
	}

	delete commentData;
}

