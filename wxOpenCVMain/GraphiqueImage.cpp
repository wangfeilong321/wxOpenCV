#include "FenetrePrincipale.h"
#include "wx/image.h"
#include "imagestat.h"

#include <fstream>
using namespace std;


wxColor		*FenetrePrincipale::pLineaire=NULL;
wxColor		*FenetrePrincipale::pAleatoire=NULL;
wxColor		*FenetrePrincipale::pJet=NULL;
wxColor		*FenetrePrincipale::pRainbow=NULL;
wxColor		*FenetrePrincipale::pPerso=NULL;
wxColor		*FenetrePrincipale::pPersoInv=NULL;
wxColor		*FenetrePrincipale::pLin256Boucle=NULL;
wxColor		*FenetrePrincipale::pRainbow256Boucle=NULL;
wxColor		*FenetrePrincipale::pLin256=NULL;
wxColor		*FenetrePrincipale::pRainbow256=NULL;
wxColor		*FenetrePrincipale::pThermique=NULL;
wxColor		*FenetrePrincipale::pThermique256=NULL;
wxColor		*FenetrePrincipale::pThermique256Boucle=NULL;
int			FenetrePrincipale::nbObjetFenetrePrincipale=0;

void FenetrePrincipale::DefSeuilNivBas(double x,int plan)
{
if (plan==-1)
	for (int i=0;i<imAcq->channels();i++)
		seuilNivBas[i]=x;
else
		seuilNivBas[plan]=x;
}


void FenetrePrincipale::DefCoeffCanal(double x,int plan)
{
if (plan==-1)
	for (int i=0;i<imAcq->channels();i++)
		coeffCanal[i]=x;
else
		coeffCanal[plan]=x;
}


void FenetrePrincipale::TracerDIB(wxImage *d, wxBufferedPaintDC &hdc, wxPoint *curseur)
{
int x=feuille->GetScrollPos(wxHORIZONTAL);
int y=feuille->GetScrollPos(wxVERTICAL);
if (curseur) 
	{
	wxPoint s(curseur->x,curseur->y);
	osgApp->TracerZoom(s);
	return;
	}
hdc.SetBrush(*wxRED_BRUSH);
//if (facteurZoom!=0)
	{
	if (imAcq->cols>0)
		{
		if (feuille->BitmapAffichee()==NULL)
			{
			wxRect		src(0,0,imAcq->cols,imAcq->rows);
			wxRect		dst;
			int			fZoomNume,fZoomDeno;

			CalculZoom(fZoomNume,fZoomDeno);
			dst=wxRect(0,0,max(imAcq->cols*fZoomNume/fZoomDeno-1,1),max(imAcq->rows*fZoomNume/fZoomDeno-1,1));
			wxImage		sousImage=d->GetSubImage(src);
			wxImage		imageZoom=sousImage.Scale(dst.GetRight()-dst.GetLeft()+1,dst.GetBottom()-dst.GetTop()+1);

			feuille->BitmapAffichee(new wxBitmap(imageZoom));
			}
		hdc.DrawBitmap(*feuille->BitmapAffichee(),0,0);
		}
	}
/*else
	{
	wxBitmap pub(*d);
	if (curseur)
		osgApp->TracerZoom(*curseur);

	else
		{
		hdc.DrawBitmap(pub,-x,-y);
		hdc.SetPen(*wxBLACK);
		hdc.DrawRectangle(0,0,r.GetRight(),r.GetBottom());
		hdc.DrawRectangle(r.GetLeft(),dst.GetBottom(),r.GetRight(),r.GetBottom());
		}
	}*/
}

void FenetrePrincipale::OnPaint(wxPaintEvent& event)
{
wxPaintDC	hdc(this);
//DrawWindow(hdc);
}

void FenetrePrincipale::DrawWindow(wxBufferedPaintDC &hdc)
{
    if (imAffichee->GetHeight() != imAcq->rows || imAffichee->GetWidth() != imAcq->cols)
    {
        imAffichee = NULL;
        delete tabRGB;
        tabRGB = NULL;
    }
if (!imAcq)
	return;
if (!imAffichee)
	if (imAcq)
		DIB(imAcq);
if (imAffichee)
	{
	wxCriticalSectionLocker enter(travailCam);
	TracerDIB(imAffichee, hdc);
	}
if (feuille->ModeRectangle())
	DrawRectangles(hdc);
if (feuille->ModeCoupe())
	DrawLignes(hdc);


TracerContour(hdc);
TracerContourPoly(hdc);
TracerEnveloppe(hdc);
TracerDefautEnveloppe(hdc);
TracerLigneHough(hdc);
TracerLigneProbaHough(hdc);
TracerCercleHough(hdc);
TracerBonCoin(hdc);
TracerRegionMvt(hdc);
TracerPointBLOB(hdc);
if(feuille->PtCtrl())
    feuille->DrawShapes(hdc);

if (cam && cam->IsRunning())
	{
	wxCriticalSectionLocker enter(travailCam);

    TracerPointORB(hdc);
    TracerPointSIFT(hdc);
    TracerPointSURF(hdc);
    TracerPointFREAK(hdc);
    TracerPointBRISK(hdc);
    TracerPointKAZE(hdc);
    TracerPointAKAZE(hdc);
    TracerPointAGAST(hdc);
    TracerPointMSER(hdc);
	TracerFlotOptique(hdc);
    if (statActif)
        imgStatIm->Plot(modeCamera);
    }
else
    {

    TracerPointORB(hdc);
    TracerPointSIFT(hdc);
    TracerPointSURF(hdc);
    TracerPointFREAK(hdc);
    TracerPointBRISK(hdc);
    TracerPointKAZE(hdc);
    TracerPointAKAZE(hdc);
    TracerPointAGAST(hdc);
    TracerPointMSER(hdc);
    if (statActif)
        imgStatIm->Plot(modeCamera);
    TracerPointMSER(hdc);
    TracerFlotOptique(hdc);
    }
}

void FenetrePrincipale::DrawRectangles(wxBufferedPaintDC &hdc)
{
wxBrush wt=*wxTRANSPARENT_BRUSH;
hdc.SetBrush(wt);
hdc.SetLogicalFunction(wxXOR);
hdc.SetPen(wxPen(*wxWHITE, 3));
for (int i=0;i<10;i++)
	if (feuille->Rectangle(i)->GetWidth()!=0 || feuille->Rectangle(i)->GetHeight()!=0)
		{
		hdc.DrawRectangle(RepereImageEcran(*feuille->Rectangle(i)));
		if (i==feuille->IndiceRectangleSelec())
			{
			int		fZoomNume,fZoomDeno;

			CalculZoom(fZoomNume,fZoomDeno);
			wxPoint pTmp(feuille->Rectangle(i)->GetTopLeft());
			wxPoint p1(RepereImageEcran(pTmp));
			wxRect rTrace(p1.x,p1.y,feuille->Rectangle(i)->width*fZoomNume/fZoomDeno,feuille->Rectangle(i)->height*fZoomNume/fZoomDeno);
			hdc.DrawLine(rTrace.GetBottomLeft(),rTrace.GetTopRight());
			hdc.DrawLine(rTrace.GetTopLeft(),rTrace.GetBottomRight());
			}
		}
}

void FenetrePrincipale::DrawLignes(wxBufferedPaintDC &hdc)
{
wxBrush wt=*wxTRANSPARENT_BRUSH;
hdc.SetBrush(wt);
wxPen pActif(*wxBLACK, 3);
wxPen pInactif(*wxBLACK, 1);
for (int i=0;i<10;i++)
	if (feuille->Coupe(i)->GetWidth()!=0 || feuille->Coupe(i)->GetHeight()!=0)
		{
		wxPoint pTmp1(feuille->Coupe(i)->GetTopLeft());
		wxPoint pTmp2(feuille->Coupe(i)->GetBottomRight());
		if (i==feuille->IndiceCoupeSelec())
			hdc.SetPen(pActif);
		else
			hdc.SetPen(pInactif);
		hdc.DrawLine(RepereImageEcran(pTmp1), RepereImageEcran(pTmp2));
	}
}

void FenetrePrincipale::TracerCercle(char *rSelect)
{
	return;


}


int	FenetrePrincipale::palArcEnCiel[1000]={
0,0,0,11,0,117,0,0,131,0,0,136,0,0,140,0,0,145,0,0,149,0,0,153,0,0,158,0,0,162,0,0,167,0,0,171,0,0,175,0,0,180,0,0,184,0,0,189,0,0,193,0,0,197,0,0,202,0,0,206,0,0,211,0,0,215,0,0,219,0,0,224,0,0,228,0,0,233,0,0,237,0,0,241,0,0,246,0,0,250,0,0,255,0,3,255,0,7,255,0,11,255,0,15,255,0,19,255,0,23,255,0,27,255,0,31,255,0,35,255,0,39,255,0,43,255,0,47,255,0,51,255,0,55,255,0,59,255,0,63,255,0,67,255,0,71,255,0,75,255,0,79,255,0,83,255,0,87,255,0,91,255,0,95,255,0,99,255,0,103,255,0,107,255,0,111,255,0,115,255,0,119,255,0,123,255,0,127,255,0,131,255,0,135,255,0,139,255,0,143,255,0,147,255,0,151,255,0,155,255,0,159,255,0,163,255,0,167,255,0,171,255,0,175,255,0,179,255,0,183,255,0,187,255,0,191,255,0,195,255,0,199,255,0,203,255,0,207,255,0,211,255,0,215,255,0,219,255,0,223,255,0,227,255,0,231,255,0,235,255,0,239,255,0,243,255,0,247,255,0,251,255,0,255,255,3,255,251,7,255,247,11,255,243,15,255,239,19,255,235,23,255,231,27,255,227,31,255,223,35,255,219,39,255,215,43,255,211,47,255,207,51,255,203,55,255,199,59,255,195,63,255,191,67,255,187,71,255,183,75,255,179,79,255,175,83,255,171,87,255,167,91,255,163,95,255,159,99,255,155,103,255,151,107,255,147,111,255,143,115,255,139,119,255,135,123,255,131,127,255,127,131,255,123,135,255,119,139,255,115,143,255,111,147,255,107,151,255,103,155,255,99,159,255,95,163,255,91,167,255,87,171,255,83,175,255,79,179,255,75,183,255,71,187,255,67,191,255,63,195,255,59,199,255,55,203,255,51,207,255,47,211,255,43,215,255,39,219,255,35,223,255,31,227,255,27,231,255,23,235,255,19,239,255,15,243,255,11,247,255,7,251,255,3,255,255,0,255,255,0,255,251,0,255,247,0,255,243,0,255,239,0,255,235,0,255,231,0,255,227,0,255,223,0,255,219,0,255,215,0,255,211,0,255,207,0,255,203,0,255,199,0,255,195,0,255,191,0,255,187,0,255,183,0,255,179,0,255,175,0,255,171,0,255,167,0,255,163,0,255,159,0,255,155,0,255,151,0,255,147,0,255,143,0,255,139,0,255,135,0,255,131,0,255,127,0,255,123,0,255,119,0,255,115,0,255,111,0,255,107,0,255,103,0,255,99,0,255,95,0,255,91,0,255,87,0,255,83,0,255,79,0,255,75,0,255,71,0,255,67,0,255,
63,0,255,59,0,255,55,0,255,51,0,255,47,0,255,43,0,255,39,0,255,35,0,255,31,0,255,27,0,255,23,0,255,19,0,255,15,0,255,11,0,255,7,0,255,3,0,255,0,0,255,0,0,251,0,0,247,0,0,243,0,0,239,0,0,235,0,0,231,0,0,227,0,0,223,0,0,219,0,0,215,0,0,211,0,0,207,0,0,203,0,0,199,0,0,195,0,0,191,0,0,187,0,0,183,0,0,179,0,0,175,0,0,171,0,0,167,0,0,163,0,0,159,0,0,155,0,0,151,0,0,147,0,0,143,0,0,139,0,0,135,0,0,131,0,0};

void FenetrePrincipale::InitPalette(int nbCouleur)
{
double pi=acos(-1.0);
long i;
nbCouleurPalette=nbCouleur;
if (!pLineaire)
	{
	pLineaire = new wxColour[nbCouleur];
	pJet = new wxColour[65536];
	pRainbow = new wxColour[65536];
	pPerso = new wxColour[65536];
	pPersoInv = new wxColour[65536];
	pRainbow256 = new wxColour[65536];
	pRainbow256Boucle = new wxColour[65536];
	pLin256 = new wxColour[65536];
	pLin256Boucle = new wxColour[65536];
	pThermique = new wxColour[65536];
	pThermique256 = new wxColour[65536];
	pThermique256Boucle = new wxColour[65536];
	pAleatoire= new wxColour[nbCouleur];
	for (i=0;i<nbCouleur;i++)
			pLineaire[i].Set(i/256,i/256,i/256);
	float red,green,blue;
	{
	ifstream ff("jetcolor16384.txt");
	if (ff.is_open())
		{
		for (int i=0;i<16384;i++)
			{
			ff>>red>>green>>blue;
			pJet[4*i].Set(red*255,green*255,blue*255);
			pJet[4*i+1].Set(red*255,green*255,blue*255);
			pJet[4*i+2].Set(red*255,green*255,blue*255);
			pJet[4*i+3].Set(red*255,green*255,blue*255);
			}
		ff.close();
		}
	}
	
	{
	ifstream ff("rainbow16384.txt");
	if (ff.is_open())
		{
		for (int i=0;i<16384;i++)
			{
			ff>>red>>green>>blue;
			pRainbow[4*i].Set(red*255,green*255,blue*255);
			pRainbow[4*i+1].Set(red*255,green*255,blue*255);
			pRainbow[4*i+2].Set(red*255,green*255,blue*255);
			pRainbow[4*i+3].Set(red*255,green*255,blue*255);
			}
		ff.close();
		}
	}
	{
	ifstream ff("palPerso.txt");
	if (ff.is_open())
		{
		for (int i=0;i<16384;i++)
			{
			ff>>red>>green>>blue;
			pPerso[4*i].Set(red*255,green*255,blue*255);
			pPerso[4*i+1].Set(pJet[4*i].Red(),pJet[4*i].Green(),pJet[4*i].Blue());
			pPerso[4*i+2].Set(pJet[4*i].Red(),pJet[4*i].Green(),pJet[4*i].Blue());
			pPerso[4*i+3].Set(pJet[4*i].Red(),pJet[4*i].Green(),pJet[4*i].Blue());
			}
		ff.close();
		}
	}
	{
	ifstream ff("palPersoInv.txt");
	if (ff.is_open())
		{
		for (int i=0;i<16384;i++)
			{
			ff>>red>>green>>blue;
			pPersoInv[4*i].Set(red*255,green*255,blue*255);
			pPersoInv[4*i+1].Set(red*255,green*255,blue*255);
			pPersoInv[4*i+2].Set(red*255,green*255,blue*255);
			pPersoInv[4*i+3].Set(red*255,green*255,blue*255);
			}
		ff.close();
		}
	for (int i=0;i<256;i++)
		{
		for (int j=0;j<256;j++)
			{
			pRainbow256[i*256+j].Set(pRainbow[i*256].Red(),pRainbow[i*256].Green(),pRainbow[i*256].Blue());
			pRainbow256Boucle[j*256+i].Set(pRainbow[i*256].Red(),pRainbow[i*256].Green(),pRainbow[i*256].Blue());
			pLin256[i*256+j].Set(pLineaire[i*256].Red(),pLineaire[i*256].Green(),pLineaire[i*256].Blue());
			pLin256Boucle[j*256+i].Set(pLineaire[i*256].Red(),pLineaire[i*256].Green(),pLineaire[i*256].Blue());
			}
		}
	}
	for (i=0;i<nbCouleur;i++)
			pAleatoire[i].Set(rand()&0xFF,rand()&0xFF,rand()&0xFF);
	
	}
pCouleur=pLineaire;
}

void FenetrePrincipale::DIB(ImageInfoCV *im)
{
if (modeTransparence==0)
	DIBImage(im,0);
else
	{
	long taille = im->cols*im->rows*3;
	int nbPlanTransparence=0;
	RAZTransparence();
	for (int i=0;i<4;i++)
		if (cTransparence[i]!=0)
			nbPlanTransparence++;
	for (int i=0;i<4;i++)
		if (cTransparence[i]!=0)
			{
			switch(i){
			case 0:
				DIBImage(imAcq,nbPlanTransparence);
				break;
				}
			nbPlanTransparence--;
			unsigned char *cSrc = (unsigned char *)tabRGB;
			unsigned char *cDst = (unsigned char *)tabRGBTransparence;
			
			float k=cTransparence[i]/100.0;
			if (nbPlanTransparence!=0)
				for (int j=0;j<taille;j++,cSrc++,cDst++)
					*cDst = *cDst + k * *cSrc;
			else
				for (int j=0;j<taille;j++,cSrc++,cDst++)
					*cSrc = *cDst + k * *cSrc;
				
			}

	}
}
/**************************************************************
Conversion d'une image UMat en DIB wxwidgets pour l'affichage
**************************************************************/

void FenetrePrincipale::DIBImage(ImageInfoCV *imSrc,int	indPlanTransparent)
{
cv::UMat *im=(cv::UMat*)imSrc;
/* Les images sont du type CV_8UC1, CV_8UC3, CV_16US */
// Les images 3D ne sont pas g�r�es
if (im->dims>2)
	return;
if (!pCouleur)
	InitPalette(65536);

// zone m�moire pour le DIB 
long taille = im->cols*im->rows*3;
if (imAffichee && imAffichee->GetHeight()*imAffichee->GetWidth() != im->cols*im->rows)
	{
	delete tabRGBTransparence;
	tabRGBTransparence=NULL;
	tabRGB=NULL;
	delete imAffichee;
	imAffichee=NULL;
	if (imGain)
		{
		correctionGain=false;
		delete imGain;
		imGain=NULL;
		}
	}

if (tabRGB==NULL)
	{
	tabRGB = new unsigned char[taille];
	tabRGBTransparence = new unsigned char[taille];
	}
unsigned char *dMasque;
bool masqueActif=false;
cv::Mat matMasque;
cv::Mat matGain;
cv::Mat matIm;
matIm = im->getMat(cv::ACCESS_READ);
if (imSrc->MasqueOperateur()->rows*imSrc->MasqueOperateur()->cols != 0)
{
    masqueActif = true;
    matMasque = imSrc->MasqueOperateur()->getMat(cv::ACCESS_READ);
    dMasque = matMasque.ptr(0);
}
int	nbCanaux=im->channels();
if (correctionGain && imGain)
    matGain = imGain->getMat(cv::ACCESS_READ);
switch(im->depth()){
case CV_32F :
	for (int i=0;i<im->rows;i++)		
		if (nbCanaux%2==1)	// Nombre r�el
			{
            if (masqueActif)
                dMasque = matMasque.ptr(i);
            float *g = NULL;
			if (correctionGain && imGain)
				g=(float*)matGain.ptr(i);
			float *d=(float*)matIm.ptr(i);
			unsigned char *debLigne = (unsigned char *)tabRGB+i*im->cols*3;
			for (int j=0;j<im->cols;j++,debLigne+=3)
				{
				for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
					{
					double v = (*d-seuilNivBas[indCanal])*coeffCanal[indCanal]; 
					if (correctionGain)
						v=*g++*v;
					if (v<0)
						v =0;
					if (v>=nbCouleurPalette)
						v=nbCouleurPalette-1;
					unsigned short val=(unsigned short)v;
                    if (masqueActif && *dMasque == 0)
                        val = val / 2;
                    if (!planActif[indCanal])
						val=0;
					switch(indCanal){
					case 0:
						debLigne[2] = pCouleur[val].Blue(); 
						if (nbCanaux==1)
							{
							debLigne[1] = pCouleur[val].Green();
							debLigne[0] = pCouleur[val].Red();
							}
						break;
					case 1:
						debLigne[1] = pCouleur[val].Green(); 
						if (nbCanaux==2)
							{
							debLigne[0] = pCouleur[val].Red();
							}
						break;
					case 2:
						debLigne[0] = pCouleur[val].Red(); 
						break;
						}
					}
                if (masqueActif)
                    dMasque++;
                }

			}
		else
			{
            if (masqueActif)
                dMasque = matMasque.ptr(i);
            complex<float> *d = (complex<float>*)matIm.ptr(i);
			unsigned char *debLigne = (unsigned char *)tabRGB+i*im->cols*3;
			for (int j=0;j<im->cols;j++,debLigne+=3)
				{
				for (int indCanal=0;indCanal<nbCanaux/2;indCanal++,d++)
					{
					double v;
					switch(feuille->ModeComplexe()){
					case 0:
						v = (abs(*d)-seuilNivBas[indCanal])*coeffCanal[indCanal]; 
						break;
					case 1:
						v = (d->real()-seuilNivBas[indCanal])*coeffCanal[indCanal]; 
						break;
					case 2:
						v = (d->imag()-seuilNivBas[indCanal])*coeffCanal[indCanal]; 
						break;
					case 3:
						v = (20*log(abs(*d)+1)-seuilNivBas[indCanal])*coeffCanal[indCanal]; 
						break;
					case 4:
						v = (atan2(d->imag(),d->real())-seuilNivBas[indCanal])*coeffCanal[indCanal]; 
						break;
						}
					if (v<0)
						v =0;
					if (v>=nbCouleurPalette)
						v=nbCouleurPalette-1;
					unsigned short val=(unsigned short)v;
                    if (masqueActif && *dMasque == 0)
                        val = val / 2;
                    if (!planActif[indCanal])
						val=0;
					switch(indCanal){
					case 0:
						debLigne[2] = pCouleur[val].Blue(); 
						if (nbCanaux/2==1)
							{
							debLigne[1] = pCouleur[val].Green();
							debLigne[0] = pCouleur[val].Red();
							}
						break;
					case 1:
						debLigne[1] = pCouleur[val].Green(); 
						if (nbCanaux/2==2)
							{
							debLigne[0] = pCouleur[val].Red();
							}
						break;
					case 2:
						debLigne[0] = pCouleur[val].Red(); 
						break;
						}
					}
                if (masqueActif)
                    dMasque++;
                }
			}
	break;
case CV_32S :
	for (int i=0;i<im->rows;i++)		
		{
        if (masqueActif)
            dMasque = matMasque.ptr(i);
        long *d = (long*)matIm.ptr(i);
		unsigned char *debLigne = (unsigned char *)tabRGB+i*im->cols*3;
		float *g=NULL;
		if (correctionGain && imGain)
			g=(float*)matGain.ptr(i);
		for (int j=0;j<im->cols;j++,debLigne+=3)
			{
			for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				{
				double v = (*d-seuilNivBas[indCanal])*coeffCanal[indCanal]; 
				if (correctionGain)
					v=*g++*v;
                if (v<0)
					v =0;
				if (v>=nbCouleurPalette)
					v=nbCouleurPalette-1;
				unsigned short val=(unsigned short)v;
                if (masqueActif && *dMasque == 0)
                    val = val / 2;
                if (!planActif[indCanal])
					val=0;
				switch(indCanal){
				case 0:
					debLigne[2] = pCouleur[val].Blue(); 
					if (nbCanaux==1)
						{
						debLigne[1] = pCouleur[val].Green();
						debLigne[0] = pCouleur[val].Red();
						}
					break;
				case 1:
					debLigne[1] = pCouleur[val].Green(); 
					if (nbCanaux==2)
						{
						debLigne[0] = pCouleur[val].Red();
						}
					break;
				case 2:
					debLigne[0] = pCouleur[val].Red(); 
					break;
					}
				}
            if (masqueActif)
                dMasque++;
            }
		}
	break;
case CV_16U :
	for (int i=0;i<im->rows;i++)		
		{
        if (masqueActif)
            dMasque = matMasque.ptr(i);
        unsigned short *d = (unsigned short*)matIm.ptr(i);
		unsigned char *debLigne = (unsigned char *)tabRGB+i*im->cols*3;
		float *g=NULL;
		if (correctionGain && imGain)
			g=(float*)matGain.ptr(i);
		for (int j=0;j<im->cols;j++,debLigne+=3)
			{
			for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				{
				double v = (*d-seuilNivBas[indCanal])*coeffCanal[indCanal]; 
				if (correctionGain)
					v=*g++*v;
				if (v<0)
					v =0;
				if (v>=nbCouleurPalette)
					v=nbCouleurPalette-1;
				unsigned short val=(unsigned short)v;
                if (masqueActif && *dMasque == 0)
                    val = val / 2;
                if (!planActif[indCanal])
					val=0;
				switch(indCanal){
				case 0:
					debLigne[2] = pCouleur[val].Blue(); 
					if (nbCanaux==1)
						{
						debLigne[1] = pCouleur[val].Green();
						debLigne[0] = pCouleur[val].Red();
						}
					break;
				case 1:
					debLigne[1] = pCouleur[val].Green(); 
					if (nbCanaux==2)
						{
						debLigne[0] = pCouleur[val].Red();
						}
					break;
				case 2:
					debLigne[0] = pCouleur[val].Red(); 
					break;
					}
				}
            if (masqueActif)
                dMasque++;
            }
		}
	break;
case CV_16S :
	for (int i=0;i<im->rows;i++)		
		{
        if (masqueActif)
            dMasque = matMasque.ptr(i);
        short *d = (short*)matIm.ptr(i);
		unsigned char *debLigne = (unsigned char *)tabRGB+i*im->cols*3;
		float *g=NULL;
		if (correctionGain && imGain)
			g=(float*)matGain.ptr(i);
		for (int j=0;j<im->cols;j++,debLigne+=3)
			{
			for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				{
				double v = (*d-seuilNivBas[indCanal])*coeffCanal[indCanal]; 
				if (correctionGain)
					v=*g++*v;
				if (v<0)
					v =0;
				if (v>=nbCouleurPalette)
					v=nbCouleurPalette-1;
				unsigned short val=(unsigned short)v;
                if (masqueActif && *dMasque==0)
                    val = val / 2;
                if (!planActif[indCanal])
					val=0;
				switch(indCanal){
				case 0:
					debLigne[2] = pCouleur[val].Blue(); 
					if (nbCanaux==1)
						{
						debLigne[1] = pCouleur[val].Green();
						debLigne[0] = pCouleur[val].Red();
						}
					break;
				case 1:
					debLigne[1] = pCouleur[val].Green(); 
					if (nbCanaux==2)
						{
						debLigne[0] = pCouleur[val].Red();
						}
					break;
				case 2:
					debLigne[0] = pCouleur[val].Red(); 
					break;
					}
				}
            if (masqueActif)
                dMasque++;
            }
		}
	break;
case CV_8U :
	for (int i=0;i<im->rows;i++)		
		{
        if (masqueActif)
            dMasque = matMasque.ptr(i);
        unsigned char *d = matIm.ptr(i);
		unsigned char *debLigne = (unsigned char *)tabRGB+i*im->cols*3;
		float *g=NULL;
		if (correctionGain && imGain)
			g=(float*)matGain.ptr(i);
		for (int j=0;j<im->cols;j++,debLigne+=3)
			{
			for (int indCanal=0;indCanal<nbCanaux;indCanal++,d++)
				{
				double v = (*d-seuilNivBas[indCanal])*coeffCanal[indCanal]; 
				if (correctionGain)
					v=*g++*v;
				if (v<0)
					v =0;
				if (v>=nbCouleurPalette)
					v=nbCouleurPalette-1;
				unsigned short val=(unsigned short)v;
                if (masqueActif && *dMasque==0)
                    val = val / 2;
				if (!planActif[indCanal])
					val=0;
				switch(indCanal){
				case 0:
					debLigne[2] = pCouleur[val].Blue(); 
					if (nbCanaux==1)
						{
						debLigne[1] = pCouleur[val].Green();
						debLigne[0] = pCouleur[val].Red();
						}
					break;
				case 1:
					debLigne[1] = pCouleur[val].Green(); 
					if (nbCanaux==2)
						{
						debLigne[0] = pCouleur[val].Red();
						}
					break;
				case 2:
					debLigne[0] = pCouleur[val].Red(); 
					break;
					}
				}
            if (masqueActif)
                dMasque++;
            }
		}
	}

if (imAffichee==NULL)
	{

//	imAffichee=new wxImage((int)im->cols,(int)im->rows);
//	delete imAffichee;
	if (taille==0)
		imAffichee=new wxImage(1,1);
	else
		imAffichee=new wxImage((int)im->cols,(int)im->rows);
	imAffichee->SetMask(0);
	imAffichee->SetData(tabRGB);
	}
return;
}


wxColor FenetrePrincipale::LitCouleurPixel(wxPoint &p)
{
if (!imAffichee)
		return wxColor(0,0,0);
wxColor r(imAffichee->GetRed(p.x,p.y),imAffichee->GetGreen(p.x,p.y),imAffichee->GetBlue(p.x,p.y));
return r;
}


void FenetrePrincipale::RAZTransparence()
{
if (!tabRGB)
	return;
long taille = imAcq->cols*imAcq->rows*3;

unsigned char *debLigne = (unsigned char *)tabRGBTransparence;

for (int i=0;i<taille;i++,debLigne++)
	*debLigne=0;		
return;
}


void FenetrePrincipale::CalculZoom(int &nume,int &deno)
{
deno=1;
switch(feuille->FacteurZoom()){
case -3 :
	nume=1;
	deno=8;
	break;
case -2 :
	nume=1;
	deno=4;
	break;
case -1 :
	nume=1;
	deno=2;
	break;
case 0 :
	nume=1;
	deno=1;
	break;
case 1 :
	nume=2;
	deno=1;
	break;
case 2 :
	nume=4;
	deno=1;
	break;
case 3 :
	nume=8;
	deno=1;
	break;
default :
	deno=1;
	nume=1;
	}
}	


wxRect FenetrePrincipale::CalculPosRectDst(wxRect &src,wxPoint *p)
{
wxRect	recSrc(0,0,imAcq->cols,imAcq->rows);
wxRect	rMax;
long	mini=0,maxi;
long	origX=0,origY=0;
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);

rMax=feuille->GetClientRect();
rMax=src;
maxi=feuille->GetScrollRange(wxVERTICAL);
wxPoint	p1(rMax.GetLeft()/fZoomDeno*fZoomDeno,rMax.GetTop()/fZoomDeno*fZoomDeno);
wxPoint	p2(	(rMax.GetRight()+fZoomNume-1)/fZoomDeno*fZoomDeno,
			(rMax.GetBottom()+fZoomNume-1)/fZoomDeno*fZoomDeno);

wxPoint	p1Img=RepereEcranImage(p1);
wxPoint	p2Img=RepereEcranImage(p2);
p2Img.x += fZoomDeno-1;
p2Img.y += fZoomDeno-1;
if (!recSrc.Contains(p1Img))
	{
	p1Img.x = recSrc.GetRight();
	p1Img.y = recSrc.GetBottom();
	p1 = RepereImageEcran(p1Img);
	}
if (!recSrc.Contains(p2Img))
	{
	p2Img.x = recSrc.GetRight();
	p2Img.y = recSrc.GetBottom();
	p2 = RepereImageEcran(p2Img);
	}
wxRect dst(p1,p2);
src=wxRect(p1Img,p2Img);
if (p==NULL)
	return dst;
long largeur = p2Img.x - p1Img.x;
long hauteur = p2Img.y - p1Img.y;
origX = p->x-largeur/2;
long right = p->x+largeur/2;
origY = p->y-hauteur/2;
long bottom = p->y+hauteur/2;
if (recSrc.GetRight()<right)
	{
	right= recSrc.GetRight();
	origX = right-largeur;
	}
if (recSrc.GetBottom()<bottom)
	{
	bottom= recSrc.GetBottom();
	origY = bottom-hauteur;
	}
if (recSrc.GetLeft()>origX)
	{
	origX= recSrc.GetLeft();
	right = origX+largeur;
	}
if (recSrc.GetTop()>origY)
	{
	origY= recSrc.GetTop();
	bottom = origY +hauteur;
	}
src=wxRect(origX,origY,right,bottom);
return dst;
}


wxPoint FenetrePrincipale::RepereEcranImage(wxPoint &p)
{
wxRect	recSrc(0,0,imAcq->cols,imAcq->rows);
wxPoint pt(0,0);
long	mini=0,maxi;
int		x=feuille->GetScrollPos(wxHORIZONTAL);
int		y=feuille->GetScrollPos(wxVERTICAL);
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);

maxi=feuille->GetScrollRange(wxVERTICAL);
pt.x = (long)((p.x)*fZoomDeno/fZoomNume+x+recSrc.GetLeft());
pt.y = (long)((p.y)*fZoomDeno/fZoomNume+recSrc.GetTop()+y);

return pt;
}

wxPoint FenetrePrincipale::RepereImageEcran(wxPoint &p)
{
wxRect	recSrc(0,0,imAcq->cols,imAcq->rows);
wxPoint pt(0,0);
long	maxi;
int		x=feuille->GetScrollPos(wxHORIZONTAL)*0;
int		y=feuille->GetScrollPos(wxVERTICAL)*0;
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);

maxi=feuille->GetScrollRange(wxVERTICAL);
	pt.x = (long)((p.x-x-recSrc.GetLeft())*fZoomNume/fZoomDeno);
	pt.y = (long)((p.y-recSrc.GetTop()-y)*fZoomNume/fZoomDeno);

return pt;
}

wxRect FenetrePrincipale::RepereImageEcran(wxRect &r)
{
wxPoint	p1Img(r.GetLeftTop()),p2Img(r.GetBottomRight());
wxPoint	p1=RepereImageEcran(p1Img);
wxPoint	p2=RepereImageEcran(p2Img);
return wxRect(p1,p2);
}
wxRect FenetrePrincipale::RepereEcranImage(wxRect &r)
{
wxPoint	p1(r.GetLeftTop()),p2(r.GetBottomRight());
wxPoint	p1Img=RepereEcranImage(p1);
wxPoint	p2Img=RepereEcranImage(p2);
return wxRect(p1Img,p2Img);
}
