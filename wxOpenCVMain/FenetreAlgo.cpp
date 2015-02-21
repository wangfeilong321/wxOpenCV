#include "FenetreAlgo.h"
#include "imagestat.h"
#include <wx/hyperlink.h>

using namespace std;

void FenetrePrincipale::ParamAlgo(wxCommandEvent& event)
{
if (!fenAlgo==NULL)
	return;
fenAlgo=new FenetreAlgo(this,origineImage.nomOperation,wxPoint(530,0), wxSize(430,570),this->osgApp);
fenAlgo->DefFenMere(this);
fenAlgo->DefOSGApp(this->osgApp);
fenAlgo->Show(true);
fenAlgo->Refresh(true);
fenAlgo->SendSizeEvent   ();
}


BEGIN_EVENT_TABLE(FenetreAlgo, wxFrame)
    EVT_CLOSE(FenetreAlgo::OnClose)
//    EVT_SPINCTRL(-1,FenetreAlgo::OnSpinEntier) 
    EVT_SPINCTRLDOUBLE(-1,FenetreAlgo::OnSpinReel) 

END_EVENT_TABLE()

 
FenetreAlgo::FenetreAlgo(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size,wxOsgApp *osg, long style)
     : wxFrame(frame, wxID_ANY, title, pos, size, wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{
panneau=new wxPanel(this, wxID_ANY,  pos, size, wxTAB_TRAVERSAL  | wxNO_BORDER | wxNO_FULL_REPAINT_ON_RESIZE);
int hMax=0,lMax=0;
Parametre *pOCV=frame->ParamOCV();
fenMere= frame;
osgApp=osg;

classeur = new wxNotebook(panneau, wxID_ANY);
wxBoxSizer *m_sizerFrame = new wxBoxSizer(wxVERTICAL);
panneau->SetSizer(m_sizerFrame);
m_sizerFrame->Insert(0, classeur, 5, wxEXPAND | wxALL, 4);                 
m_sizerFrame->Hide(classeur);                                                   
m_sizerFrame->Show(classeur);                                                                           
panneau->Show(true);
FenetrePrincipale *f=fenMere;
nbEtape=0;
nbParamMax=0;
while(f && f->OrigineImage()->indOp1>=0)
	{
	int id=f->OrigineImage()->indOp1;
	if (id>=0)
		{
		int nbParam=f->OrigineImage()->pOCV.intParam.size();
		nbParam+=f->OrigineImage()->pOCV.doubleParam.size();
		nbParam+=2*f->OrigineImage()->pOCV.pointParam.size();
		nbParam+=2*f->OrigineImage()->pOCV.sizeParam.size();
		if (nbParamMax<nbParam)
			nbParamMax=nbParam;
		nbEtape++;
		f=((wxOsgApp *)osgApp)->Fenetre(id);
		}
	else 
		f=NULL;
	}
nbParamMax=2*(nbParamMax+2);
f=fenMere;
int nb=nbEtape-1;
listeOp.resize(nbEtape);
while(f && f->OrigineImage()->indOp1>=0)
	{
	if (f->OrigineImage())
		{
		listeOp[nb]=std::pair< Operation*,int>(f->OrigineImage(),f->IdFenetre()) ;
		wxWindow *w=CreerOngletEtape(classeur,nb);
		listeOnglet[w]=std::pair<wxString,int>(f->OrigineImage()->nomOperation,nb);
		wxString nom(_("Etape"));
		nom.Printf("%s %d : %s",nom,nb,f->OrigineImage()->nomOperation);
		classeur->InsertPage(0,w,nom,nbEtape==1);
		w->Refresh(true);
		int id=f->OrigineImage()->indOp1;
		if (id>=0)
			f=((wxOsgApp *)osgApp)->Fenetre(id);
		else 
			f=NULL;
		nb--;
		}
	else 
		f=NULL;
	}
}

wxWindow *FenetreAlgo::CreerOngletEtape(wxNotebook *classeur,int indOp)
{
wxWindow *page = new wxWindow(classeur,-1);
Parametre *pOCV=&listeOp[indOp].first->pOCV;
int nbParam=1;
int ligne=50;
int indOriCtrl=1+indOp*nbParamMax;
new wxHyperlinkCtrl (page,indOriCtrl," OpenCV Documentation",pOCV->lienHtml,wxPoint(10,10),wxSize(400,20));
new wxHyperlinkCtrl(page,indOriCtrl+1,"PDF Documentation",pOCV->refPDF,wxPoint(10,30),wxSize(400,20));
std::map<std::string,DomaineParametre<cv::Size> >::iterator its;
for (its=pOCV->sizeParam.begin();its!=pOCV->sizeParam.end();its++)
	{
	wxString nombre;
	nombre.Printf("%d",its->second.valeur.width);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	new wxStaticText(page,indOriCtrl+2*nbParam,its->first,p, s);
	p += wxPoint(s.GetX(),0);
//	wxSpinCtrl *spw=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	wxSpinCtrlDouble *spw=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	spw->SetRange(0,256); 
	spw->SetIncrement((double)its->second.pas.width); 
	spw->SetRange(0,256); 
	p += wxPoint(s.GetX(),0);
	nbParam++;
	new wxStaticText(page,indOriCtrl+2*nbParam,its->first,p, s);
	p += wxPoint(s.GetX(),0);
	nombre.Printf("%d",its->second.valeur.height);
//	wxSpinCtrl *sph=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
//	sph->SetRange(0,256); 
	wxSpinCtrlDouble *sph=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	sph->SetRange(0,256); 
	sph->SetIncrement((double)its->second.pas.height); 
		
	nbParam++;
	ligne+=20;
	}
std::map<std::string,DomaineParametre<int> >::iterator iti;
for (iti=pOCV->intParam.begin();iti!=pOCV->intParam.end();iti++)
	{
	wxString nombre;
	nombre.Printf("%d",iti->second.valeur);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	new wxStaticText(page,indOriCtrl+2*nbParam,iti->first,p, s);
	p += wxPoint(s.GetX(),0);
//	wxSpinCtrl *sp=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
//	sp->SetRange(iti->second.mini,iti->second.maxi); 
	wxSpinCtrlDouble *sp=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	sp->SetRange(iti->second.mini,iti->second.maxi); 
	sp->SetIncrement(iti->second.pas); 
	nbParam++;
	ligne+=20;
	}
std::map<std::string,DomaineParametre<double> >::iterator itd;
for (itd=pOCV->doubleParam.begin();itd!=pOCV->doubleParam.end();itd++)
	{
	wxString nombre;
	nombre.Printf("%f",itd->second.valeur);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	new wxStaticText(page,indOriCtrl+2*nbParam,itd->first,p, s);
	p += wxPoint(s.GetX(),0);
	wxSpinCtrlDouble *sp=new wxSpinCtrlDouble(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS ); 
	sp->SetRange(itd->second.mini,itd->second.maxi); 
	sp->SetIncrement(itd->second.pas); 
	nbParam++;
	ligne+=20;
	}
SetClientSize(250,ligne+20);
return page;
}


FenetreAlgo::~FenetreAlgo()
{
fenMere->RAZFenAlgo();

}

void FenetreAlgo::OnActivate(wxActivateEvent& event)
{
}

void FenetreAlgo::OnPaint(wxPaintEvent& event)
{
}

void FenetreAlgo::OnSpinEntier(wxSpinEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

Parametre *pOCV=&listeOp[ind].first->pOCV;
wxStaticText *st=(wxStaticText*)wxWindow::FindWindowById(w.GetId()-1,this);
nom=st->GetLabel();
if (pOCV->intParam.find(nom)!=pOCV->intParam.end())
	{
	int ds=pOCV->intParam[nom].valeur-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
	if (ds==0)
		return;
	int valeur=pOCV->intParam[nom].maxi+1;
	if(ds<0 )
		valeur = pOCV->intParam[nom].valeur + pOCV->intParam[nom].pas;
	if(ds>0 )
		valeur = pOCV->intParam[nom].valeur - pOCV->intParam[nom].pas;
	if (	valeur >=pOCV->intParam[nom].mini && valeur <pOCV->intParam[nom].maxi && 
		(valeur%pOCV->intParam[nom].pas)==(pOCV->intParam[nom].mini%pOCV->intParam[nom].pas))
		{
		pOCV->intParam[nom].valeur=valeur;
		((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->intParam[nom].valeur);
			ExecuterOperation(ind);
		}
	else
		wxMessageBox("Selected value is out of range or forbidden");
	}
if (pOCV->sizeParam.find(nom)!=pOCV->sizeParam.end())
	{
	if ((w.GetId()-1)%4==0)
		{
		if (pOCV->sizeParam[nom].valeur.width==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		int ds=pOCV->sizeParam[nom].valeur.width-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
		if (ds==0)
			return;
		int valeur=pOCV->sizeParam[nom].maxi.width+1;
		if(ds<0 )
			valeur = pOCV->sizeParam[nom].valeur.width + pOCV->sizeParam[nom].pas.width;
		if(ds>0 )
			valeur = pOCV->sizeParam[nom].valeur.width - pOCV->sizeParam[nom].pas.width;
		if (	valeur >=pOCV->sizeParam[nom].mini.width && valeur <pOCV->sizeParam[nom].maxi.width && 
			(valeur%pOCV->sizeParam[nom].pas.width)==(pOCV->sizeParam[nom].mini.width%pOCV->sizeParam[nom].pas.width))
			{
			pOCV->sizeParam[nom].valeur.width=valeur;
			((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.width);
			ExecuterOperation(ind);
			}
		}
	else
		{
		if (pOCV->sizeParam[nom].valeur.height==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		int ds=pOCV->sizeParam[nom].valeur.height-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
		if (ds==0)
			return;
		int valeur=pOCV->sizeParam[nom].maxi.height+1;
		if(ds<0 )
			valeur = pOCV->sizeParam[nom].valeur.height + pOCV->sizeParam[nom].pas.height;
		if(ds>0 )
			valeur = pOCV->sizeParam[nom].valeur.height - pOCV->sizeParam[nom].pas.height;
		if (	valeur >=pOCV->sizeParam[nom].mini.height && valeur <pOCV->sizeParam[nom].maxi.height && 
			(valeur%pOCV->sizeParam[nom].pas.height)==(pOCV->sizeParam[nom].mini.height%pOCV->sizeParam[nom].pas.height))
			{
			pOCV->sizeParam[nom].valeur.height=valeur;
			((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.height);
			ExecuterOperation(ind);

			}
		}
	}

}

void FenetreAlgo::OnSpinReel(wxSpinDoubleEvent &w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

Parametre *pOCV=&listeOp[ind].first->pOCV;
wxStaticText *st=(wxStaticText*)wxWindow::FindWindowById(w.GetId()-1,this);
nom=st->GetLabel();
if (pOCV->doubleParam.find(nom)!=pOCV->doubleParam.end())
	{
	if (pOCV->doubleParam[nom].valeur==((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
		return;
	pOCV->doubleParam[nom].valeur=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
if (pOCV->intParam.find(nom)!=pOCV->intParam.end())
	{
	if (pOCV->intParam[nom].valeur==((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
		return;
	pOCV->intParam[nom].valeur=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
if (pOCV->sizeParam.find(nom)!=pOCV->sizeParam.end())
	{
	if ((w.GetId())%4==0)
		{
		if (pOCV->sizeParam[nom].valeur.width==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->sizeParam[nom].valeur.width=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
		}
	else
		{
		if (pOCV->sizeParam[nom].valeur.height==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		pOCV->sizeParam[nom].valeur.height=((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
		}
	}
ExecuterOperation(ind);
}

void  FenetreAlgo::OnSpinPlus(wxSpinEvent& w)
{

}

void  FenetreAlgo::OnSpinMoins(wxSpinEvent& w)
{
wxOsgApp *app=(wxOsgApp *)osgApp;
if (!osgApp)
	return;
string nom;
int ind=listeOnglet[classeur->GetCurrentPage()].second;

Parametre *pOCV=&listeOp[ind].first->pOCV;
wxStaticText *st=(wxStaticText*)wxWindow::FindWindowById(w.GetId()-1,this);
nom=st->GetLabel();
if (pOCV->intParam.find(nom)!=pOCV->intParam.end())
	{
	int ds=pOCV->intParam[nom].valeur-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
	if (ds==0)
		return;
	int valeur=pOCV->intParam[nom].maxi+1;
	if(ds<0 )
		valeur = pOCV->intParam[nom].valeur + pOCV->intParam[nom].pas;
	if(ds>0 )
		valeur = pOCV->intParam[nom].valeur - pOCV->intParam[nom].pas;
	if (	valeur >=pOCV->intParam[nom].mini && valeur <pOCV->intParam[nom].maxi && 
		(valeur%pOCV->intParam[nom].pas)==(pOCV->intParam[nom].mini%pOCV->intParam[nom].pas))
		{
		pOCV->intParam[nom].valeur=valeur;
		((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->intParam[nom].valeur);
			ExecuterOperation(ind);
		}
	else
		wxMessageBox("Selected value is out of range or forbidden");
	}
if (pOCV->sizeParam.find(nom)!=pOCV->sizeParam.end())
	{
	if ((w.GetId()-1)%4==0)
		{
		if (pOCV->sizeParam[nom].valeur.width==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		int ds=pOCV->sizeParam[nom].valeur.width-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
		if (ds==0)
			return;
		int valeur=pOCV->sizeParam[nom].maxi.width+1;
		if(ds<0 )
			valeur = pOCV->sizeParam[nom].valeur.width + pOCV->sizeParam[nom].pas.width;
		if(ds>0 )
			valeur = pOCV->sizeParam[nom].valeur.width - pOCV->sizeParam[nom].pas.width;
		if (	valeur >=pOCV->sizeParam[nom].mini.width && valeur <pOCV->sizeParam[nom].maxi.width && 
			(valeur%pOCV->sizeParam[nom].pas.width)==(pOCV->sizeParam[nom].mini.width%pOCV->sizeParam[nom].pas.width))
			{
			pOCV->sizeParam[nom].valeur.width=valeur;
			((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.width);
			ExecuterOperation(ind);
			}
		}
	else
		{
		if (pOCV->sizeParam[nom].valeur.height==((wxSpinCtrl*)(w.GetEventObject()))->GetValue())
			return;
		int ds=pOCV->sizeParam[nom].valeur.height-((wxSpinCtrl*)(w.GetEventObject()))->GetValue();
		if (ds==0)
			return;
		int valeur=pOCV->sizeParam[nom].maxi.height+1;
		if(ds<0 )
			valeur = pOCV->sizeParam[nom].valeur.height + pOCV->sizeParam[nom].pas.height;
		if(ds>0 )
			valeur = pOCV->sizeParam[nom].valeur.height - pOCV->sizeParam[nom].pas.height;
		if (	valeur >=pOCV->sizeParam[nom].mini.height && valeur <pOCV->sizeParam[nom].maxi.height && 
			(valeur%pOCV->sizeParam[nom].pas.height)==(pOCV->sizeParam[nom].mini.height%pOCV->sizeParam[nom].pas.height))
			{
			pOCV->sizeParam[nom].valeur.height=valeur;
			((wxSpinCtrl*)(w.GetEventObject()))->SetValue(pOCV->sizeParam[nom].valeur.height);
			ExecuterOperation(ind);

			}
		}
	}
}

void FenetreAlgo::OnTextValider(wxCommandEvent &w)
{
}
void FenetreAlgo::OnKeyDown(wxKeyEvent &)
{
}

void FenetreAlgo::OnClose(wxCloseEvent& event)
{
fenMere->RAZFenAlgo();
wxFrame::OnCloseWindow(event);

}

void FenetreAlgo::ExecuterOperation(int indEtape)
{
if (!osgApp)
	return;
wxOsgApp	*app=(wxOsgApp *)osgApp;
ImageInfoCV **im=NULL;
for (int i=indEtape;i<nbEtape;i++)
	{
	int indFen1=app->RechercheFenetre(listeOp[i].first->op1);
	if (indFen1<0 )
		{
		wxMessageBox(_("Previous image is closed?"),_("Problem"), wxOK );
		return ;
		}
	}
for (int i=indEtape;i<nbEtape;i++)
	{
	Parametre *pOCV=&listeOp[i].first->pOCV;
	wxString  nomOperation(listeOp[i].first->nomOperation);
	app->DefOperateurImage(nomOperation);
	int indFen1=app->RechercheFenetre(listeOp[i].first->op1);
	if (indFen1<0 )
		{
		wxMessageBox(_("Previous image is closed?"),_("Problem"), wxOK );
		return ;
		}
//	if (im==NULL)
		app->DefOperande1(listeOp[i].first->op1,indFen1);
	//else
//		app->DefOperande1(im[0]);
	int indFen2=app->RechercheFenetre(listeOp[i].first->op2);
	app->DefOperande2(listeOp[i].first->op2,indFen2);
	im=app->ExecuterOperation(pOCV);
	if (im!=NULL)
		{
		FenetrePrincipale *f;

		f =app->Fenetre(listeOp[i].second);
		f->AssosierImage(im[0]);
		if (i<nbEtape-1)
			{
			listeOp[i+1].first->op1 =im[0];
			}

		f->NouvelleImage();
		f->MAJNouvelleImage();
		if (f->ImgStat())
			f->ImgStat()->Plot(true);
		f->DefHistorique();
		}
	}

}
