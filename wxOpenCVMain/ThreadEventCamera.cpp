#include "EvenementCamera.h"
#include "CameraOpenCV.h"
#include "ImageInfo.h"


ProcessGestionCamera::ProcessGestionCamera(ControleCamera *c,CameraVirtuelle *cc)
{
ctrlCam=c;
cam=cc;
}

void *ProcessGestionCamera::Entry()
{
FenetrePrincipale *parent=(FenetrePrincipale *)cam->parent;
cv::VideoCapture *captureVideo= ((CameraOpenCV*)cam)->CamVideo(); 
if (captureVideo->isOpened())
	{
	cv::Mat frameFlt1;
	cv::Mat frameFlt2;
	cv::Mat frame;
	cv::Mat frameFlt;
	ImageInfoCV *imAcq =parent->ImAcq(); 
	std::vector<cv::Point2f> repereIni,repere;
	while (!captureVideo->retrieve(frame));
	frame.convertTo(frameFlt2,CV_32FC3);
	while (!captureVideo->retrieve(frame));
	frame.convertTo(frameFlt1,CV_32FC3);
	int nbAcq=0;
	for(;nbAcq<100;)
		{
		if (captureVideo->retrieve(frame)) // get a new frame from camera
			{
			nbAcq++;
			if (parent)
				{

				frame.convertTo(frameFlt1,CV_32FC3);

				{
				wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

				for (int i=0;i<frame.rows;i++)
					{
					float *val=(float *)imAcq->ptr(i);
					float *valB1=(float *)frameFlt1.ptr(i);
					for (int j=0;j<frame.cols;j++)
						for (int k=0;k<frame.channels();k++,valB1++,val++)
							*val += *valB1 ;
						
					}
				}
				if (nbAcq%10==0)
					{
					EvtPointSuivis *x= new EvtPointSuivis(VAL_EVT_PTS_SUIVIS);
					x->ptId=repereIni;
					x->ptApp=repere;
					x->SetTimestamp(wxGetUTCTimeMillis().GetLo());
					wxQueueEvent( ((FenetrePrincipale*)parent)->GetEventHandler(), x);

					}

				}
			if (TestDestroy())
				break;
			}
		}
	}

return (wxThread::ExitCode)0;  

}


