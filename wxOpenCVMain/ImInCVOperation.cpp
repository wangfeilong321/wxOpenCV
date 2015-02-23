#include "ImageInfo.h"
#include <vector>


/**
 * @function IndOpConvolution
 * @brief Choix de l'op�rateur de convolution par son indice dans la liste des op�rateurs
 */
int		ImageInfoCV::IndOpConvolution(int i)  
{
if (i>=0)
	indOpConvolution=i;
return indOpConvolution;
}


/**
 * @function IndOpMorphologie
 * @brief Choix de l'op�rateur de morphologie par son indice dans la liste des op�rateurs
 */
int		ImageInfoCV::IndOpMorphologie(int i)
{
if (i>=0)
	indOpMorphologie=i;
return indOpMorphologie;
}

/**
 * @function Add
 * @brief Addition de deux images
 */
ImageInfoCV 	*ImageInfoCV::Add(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::add( *im1, *im2, *im,cv::noArray(),typeResultat );
return im;
}

/**
 * @function Add
 * @brief Diff�rence de deux images
 */
ImageInfoCV 	*ImageInfoCV::Sub(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::subtract( *im1, *im2, *im,cv::noArray(),typeResultat );
return im;
}

/**
 * @function Prod
 * @brief Produit de deux images
 */
ImageInfoCV 	*ImageInfoCV::Mul(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::subtract( *im1, *im2, *im,cv::noArray(), typeResultat);
return im;
}

/**
 * @function Prod
 * @brief Division de deux images
 */
ImageInfoCV 	*ImageInfoCV::Div(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::subtract( *im1, *im2, *im,cv::noArray(),typeResultat );
return im;
}


/**
 * @function Erosion
 * @brief Erosion d'une image im1 par l'op�rateur im2 o� l'op�rateur par d�faut
 */
ImageInfoCV 	*ImageInfoCV::Erosion(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (im2==NULL)
	cv::erode( *im1, *im, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
else
	cv::erode( *im1, *im, *im2 );
return im;
}

/**
 * @function Dilatation
 * @brief Dilatation d'une image im1 par l'op�rateur im2 o� l'op�rateur par d�faut
 */
ImageInfoCV 	*ImageInfoCV::Dilatation(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (im2==NULL)
	cv::dilate( *im1, *im, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
else
	cv::dilate( *im1, *im, *im2 );
return im;
}

ImageInfoCV 	*ImageInfoCV::Ouverture(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;


if (pOCV)
	{
	int nbIter;
	int borderType;
	cv::Scalar borderValue=cv::morphologyDefaultBorderValue();
	cv::Point ancrage;
	nbIter=pOCV->intParam["nbIter"].valeur;
	borderType=pOCV->intParam["borderType"].valeur;
	ancrage=pOCV->pointParam["anchor"].valeur;
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_OPEN, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_OPEN, *im2,ancrage,nbIter,borderType,borderValue);
	}
else
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_OPEN, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_OPEN, *im2);
return im;
}
 
ImageInfoCV 	*ImageInfoCV::Fermeture(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	int nbIter;
	int borderType;
	cv::Scalar borderValue=cv::morphologyDefaultBorderValue();
	cv::Point ancrage;
	nbIter=pOCV->intParam["nbIter"].valeur;
	borderType=pOCV->intParam["borderType"].valeur;
	ancrage=pOCV->pointParam["anchor"].valeur;
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_CLOSE, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_CLOSE, *im2,ancrage,nbIter,borderType,borderValue);
	}
else
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_CLOSE, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_CLOSE, *im2);
return im;
}

ImageInfoCV 	*ImageInfoCV::ChapeauHaut(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	int nbIter;
	int borderType;
	cv::Scalar borderValue=cv::morphologyDefaultBorderValue();
	cv::Point ancrage;
	nbIter=pOCV->intParam["nbIter"].valeur;
	borderType=pOCV->intParam["borderType"].valeur;
	ancrage=pOCV->pointParam["anchor"].valeur;
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_TOPHAT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_TOPHAT, *im2,ancrage,nbIter,borderType,borderValue);
	}
else
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_TOPHAT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_TOPHAT, *im2);
return im;
}

ImageInfoCV 	*ImageInfoCV::ChapeauBas(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	int nbIter;
	int borderType;
	cv::Scalar borderValue=cv::morphologyDefaultBorderValue();
	cv::Point ancrage;
	nbIter=pOCV->intParam["nbIter"].valeur;
	borderType=pOCV->intParam["borderType"].valeur;
	ancrage=pOCV->pointParam["anchor"].valeur;
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_BLACKHAT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_BLACKHAT, *im2,ancrage,nbIter,borderType,borderValue);
	}
else
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_BLACKHAT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_BLACKHAT, *im2);
return im;
}

ImageInfoCV 	*ImageInfoCV::GradMorph(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (pOCV)
	{
	int nbIter;
	int borderType;
	cv::Scalar borderValue=cv::morphologyDefaultBorderValue();
	cv::Point ancrage;
	nbIter=pOCV->intParam["nbIter"].valeur;
	borderType=pOCV->intParam["borderType"].valeur;
	ancrage=pOCV->pointParam["anchor"].valeur;
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_GRADIENT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur],ancrage,nbIter,borderType,borderValue );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_GRADIENT, *im2,ancrage,nbIter,borderType,borderValue);
	}
else
	if (im2==NULL)
		cv::morphologyEx( *im1, *im,cv::MORPH_GRADIENT, *im1->opMorph[pOCV->intParam["IndOpMorphologie"].valeur] );
	else
		cv::morphologyEx( *im1, *im,cv::MORPH_GRADIENT, *im2);
return im;
}



/**
 * @function Convolution
 * @brief Convolution d'une image im1 par l'op�rateur im2 o� l'op�rateur par d�faut
 */
ImageInfoCV 	*ImageInfoCV::Convolution(ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (im2==NULL)
	cv::filter2D( *im1, *im,typeResultat, *im1->opnn[pOCV->intParam["IndOpConvolution"].valeur] );
else
	cv::filter2D( *im1, *im,typeResultat, *im2);

im->ExtremumLoc(NULL);
return im;
}

/**
 * @function Dilatation
 * @brief Dilatation d'une image im1 par l'op�rateur im2 o� l'op�rateur par d�faut
 */
ImageInfoCV 	*ImageInfoCV::Laplacien(ImageInfoCV	*im1,Parametre &pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::Laplacian( *im1, *im, typeResultat,pOCV.intParam["ksize"].valeur,
	pOCV.doubleParam["scale"].valeur,pOCV.doubleParam["delta"].valeur,pOCV.intParam["borderType"].valeur );
return im;
}

/**
 * @function ScharrModule
 * @brief Module du gradient Scharr d'une image im1 
 */
ImageInfoCV 	*ImageInfoCV::ScharrModule(ImageInfoCV	*im1,Parametre &pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
ImageInfoCV	imx ;
ImageInfoCV	imy;
ImageInfoCV	imAbsx;
ImageInfoCV	imAbsy;

cv::Scharr( *im1, imx, pOCV.intParam["ddepth"].valeur,1,0,pOCV.doubleParam["scale"].valeur,pOCV.doubleParam["delta"].valeur,pOCV.intParam["borderType"].valeur );
cv::Scharr( *im1, imy, pOCV.intParam["ddepth"].valeur,0,1,pOCV.doubleParam["scale"].valeur,pOCV.doubleParam["delta"].valeur,pOCV.intParam["borderType"].valeur );
cv::convertScaleAbs( imx, imAbsx);
cv::convertScaleAbs( imy, imAbsy);
addWeighted( imAbsx, 0.5, imAbsy, 0.5, 0, *im );

return im;
}

/**
 * @function ScharrX
 * @brief Dilatation d'une image im1 par l'op�rateur im2 o� l'op�rateur par d�faut
 */
ImageInfoCV 	*ImageInfoCV::ScharrX(ImageInfoCV	*im1,Parametre &pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::Scharr( *im1, *im, pOCV.intParam["ddepth"].valeur,1,0,pOCV.doubleParam["scale"].valeur,pOCV.doubleParam["delta"].valeur,pOCV.intParam["borderType"].valeur );
return im;
}


/**
 * @function ScharrY
 * @brief Dilatation d'une image im1 par l'op�rateur im2 o� l'op�rateur par d�faut
 */
ImageInfoCV 	*ImageInfoCV::ScharrY(ImageInfoCV	*im1,Parametre &pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

cv::Scharr( *im1, *im, pOCV.intParam["ddepth"].valeur,0,1,
	pOCV.doubleParam["scale"].valeur,pOCV.doubleParam["delta"].valeur,pOCV.intParam["borderType"].valeur );
return im;
}



/**
 * @function Canny
 * @brief filtrage de canny d'une image im1 par l'op�rateur im2 o� l'op�rateur par d�faut
 */
ImageInfoCV 	*ImageInfoCV::Canny(ImageInfoCV	*imSrc,Parametre &pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;

if (imSrc->channels()==1)
	{
	cv::Canny(	*imSrc, *im, pOCV.doubleParam["threshold1"].valeur,
				pOCV.doubleParam["threshold2"].valeur,pOCV.intParam["aperture_size"].valeur);
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		cv::Canny( planCouleur[i], d[i], pOCV.doubleParam["threshold1"].valeur,
					pOCV.doubleParam["threshold2"].valeur,pOCV.intParam["aperture_size"].valeur);
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(), *im);
	delete []d;
	}
return im;
}

/**
 * @function Seuillage
 * @brief Seuillage d'une image imSrc 
 */
ImageInfoCV 	*ImageInfoCV::SeuillageAdaptatif(ImageInfoCV	*imSrc,Parametre &pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (imSrc->channels()==1)
	{
	cv::adaptiveThreshold( *imSrc, *im, pOCV.doubleParam["maxValue"].valeur,pOCV.intParam["adaptiveMethod"].valeur,
		pOCV.intParam["thresholdType"].valeur,pOCV.intParam["blockSize"].valeur,pOCV.doubleParam["C"].valeur);
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
	cv::adaptiveThreshold( planCouleur[i], d[i], pOCV.doubleParam["maxValue"].valeur,pOCV.intParam["adaptiveMethod"].valeur,
		pOCV.intParam["thresholdType"].valeur,pOCV.intParam["blockSize"].valeur,pOCV.doubleParam["C"].valeur);
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(), *im);
	delete []d;
	}
return im;
}

ImageInfoCV 	*ImageInfoCV::Contour(ImageInfoCV	*imSrc,Parametre &pOCV)
{
ImageInfoCV *im1=new ImageInfoCV;
*(cv::Mat*)im1= imSrc->clone();
std::vector<std::vector<cv::Point> > f;
if (imSrc->channels()==1)
	{
	cv::findContours( *im1,  f,pOCV.intParam["mode"].valeur,pOCV.intParam["method"].valeur);	
	}
 else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(),*im1);
	}
return im1;
}

void ImageInfoCV::Threshold( cv::InputArray _src, cv::OutputArray _dst, double thresh, double maxval, int type )
{
}

ImageInfoCV 	*ImageInfoCV::Seuillage(ImageInfoCV	*imSrc,Parametre &pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (imSrc->depth()==CV_16S)
	{
	pOCV.doubleParam["thresh"].maxi=32767; 
	}
if (imSrc->channels()==1)
	{
	
	threshold( *imSrc, *im, pOCV.doubleParam["thresh"].valeur,pOCV.doubleParam["maxval"].valeur,
		pOCV.intParam["threshold_type"].valeur);
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		cv::threshold(	planCouleur[i], d[i], pOCV.doubleParam["thresh"].valeur,
						pOCV.doubleParam["maxval"].valeur,pOCV.intParam["threshold_type"].valeur);
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(), *im);
	delete []d;
	}
return im;

}
ImageInfoCV 	*ImageInfoCV::LissageMoyenne(ImageInfoCV	*imSrc,Parametre &pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
cv::blur( *imSrc, *im, pOCV.sizeParam["ksize"].valeur,pOCV.pointParam["anchor"].valeur,pOCV.intParam["borderType"].valeur);

return im;
}

ImageInfoCV 	*ImageInfoCV::LissageGaussien(ImageInfoCV	*imSrc,Parametre &pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
cv::GaussianBlur( *imSrc, *im, pOCV.sizeParam["ksize"].valeur,
	pOCV.doubleParam["sigmaX"].valeur,pOCV.doubleParam["sigmaY"].valeur,
	pOCV.intParam["borderType"].valeur);
return im;
}

ImageInfoCV 	*ImageInfoCV::LissageMedian(ImageInfoCV	*imSrc,Parametre &pOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
cv::medianBlur( *imSrc, *im, pOCV.intParam["ksize"].valeur);

return im;
}


ImageInfoCV 	*ImageInfoCV::FFT(ImageInfoCV	*im1,Parametre &paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
int m = cv::getOptimalDFTSize( im1->rows );
int n = cv::getOptimalDFTSize( im1->cols ); // on the border add zero values

if (im1->channels()==1)
	{
	if (im1->type()!=CV_32FC1 && im->type()!=CV_64FC1)
		{
		ImageInfoCV	*imSrc =new ImageInfoCV;
		im1->convertTo(*imSrc, CV_32F);
		if (im1->rows!=m ||im1->cols!=n)
			{
			Mat insert;  
			cv::copyMakeBorder(*imSrc, insert, 0, m - imSrc->rows, 0, n - imSrc->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::dft(insert,*im,cv::DFT_COMPLEX_OUTPUT);
			}
		else
			cv::dft(*imSrc,*im,cv::DFT_COMPLEX_OUTPUT);
		delete imSrc;
		}
	else
		cv::dft(*im1,*im,cv::DFT_COMPLEX_OUTPUT);
		
	}
else
	{
	std::vector<Mat> planCouleurSrc;
	Mat *planCouleurDst=new Mat[im1->channels()];
	Mat *d=new Mat[im1->channels()];
	cv::split( *im1, planCouleurSrc );
	for (int i=0;i<im1->channels();i++)
		{
		planCouleurSrc[i].convertTo(planCouleurDst[i], CV_32F);
		if (im1->rows!=m ||im1->cols!=n)
			{
			Mat insert;  
			cv::copyMakeBorder(planCouleurDst[i], insert, 0, m - im1->rows, 0, n - im1->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::dft(insert,d[i],cv::DFT_COMPLEX_OUTPUT);
			}
		else
			{
			cv::dft(planCouleurDst[i],d[i],cv::DFT_COMPLEX_OUTPUT);
			}
		}
	cv::merge((const cv::Mat *)d, im1->channels(), *im);
	int nb=im->channels();
	delete []d;
	delete []planCouleurDst;

	}
return im;
}

ImageInfoCV 	*ImageInfoCV::IFFT(ImageInfoCV	*im1,Parametre &paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
int m = cv::getOptimalDFTSize( im1->rows );
int n = cv::getOptimalDFTSize( im1->cols ); // on the border add zero values

if (im1->channels()==2)
	{
	if (im1->type()!=CV_32FC2 && im->type()!=CV_64FC2)
		{
		ImageInfoCV	*imSrc =new ImageInfoCV;
		im1->convertTo(*imSrc, CV_32FC2);
		if (im1->rows!=m ||im1->cols!=n)
			{
			Mat insert;  
			cv::copyMakeBorder(*imSrc, insert, 0, m - imSrc->rows, 0, n - imSrc->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::idft(insert,*im,cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
			}
		else
			cv::idft(*imSrc,*im,cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
		delete imSrc;
		}
	else
		cv::idft(*im1,*im,cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
		
	}
else
	{
	std::vector<Mat> planCouleurSrc;
	Mat *planCouleurDst=new Mat[im1->channels()];
	Mat *d=new Mat[im1->channels()];
	cv::split( *im1, planCouleurSrc );
	for (int i=0;i<im1->channels();i++)
		{
		planCouleurSrc[i].convertTo(planCouleurDst[i], CV_32FC2);
		if (im1->rows!=m ||im1->cols!=n)
			{
			Mat insert;  
			cv::copyMakeBorder(planCouleurDst[i], insert, 0, m - im1->rows, 0, n - im1->cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
			cv::idft(insert,d[i],cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
			}
		else
			{
			cv::idft(planCouleurDst[i],d[i],cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT);
			}
		}
	cv::merge((const cv::Mat *)d, im1->channels(), *im);
	int nb=im->channels();
	delete []d;
	delete []planCouleurDst;

	}
return im;
}


ImageInfoCV *ImageInfoCV::FusionPlan(ImageInfoCV *ve,Parametre &paramOCV)
{
ImageInfoCV	*result=new ImageInfoCV;
return(result);
}

ImageInfoCV **ImageInfoCV::SeparationPlan(ImageInfoCV *im1,Parametre &paramOCV)
{
ImageInfoCV	**result=new ImageInfoCV*[im1->channels()];
std::vector<Mat> *planCouleurSrc=new std::vector<Mat>;
 
cv::split( *im1, *planCouleurSrc );
for (int i=0;i<im1->channels();i++)
	{
	result[i]=new ImageInfoCV;
	cv::merge((const cv::Mat *)&(*planCouleurSrc)[i], 1, *result[i]);
	}
paramOCV.nbImageRes=im1->channels();
return(result);
}


ImageInfoCV *ImageInfoCV::ComposanteConnexe(ImageInfoCV *im1,Parametre &paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (im1->channels()==1)
	{
	if (im->statComposante==NULL)
		{
		im->statComposante = new cv::Mat*[im1->channels()]; 
		im->centreGComposante = new cv::Mat*[im1->channels()]; 
		im->contours = new std::vector<std::vector<cv::Point> >[im1->channels()]; 
		im->arbreContour = new std::vector<cv::Vec4i> [im1->channels()]; 

		for (int i=0;i<im1->channels();i++)
			{
			im->statComposante[i] = new cv::Mat; 
			im->centreGComposante[i] = new cv::Mat; 
			}
		}

	connectedComponentsWithStats(*im1, *im,*(im->statComposante[0]),*(im->centreGComposante[0]), paramOCV.intParam["connectivity"].valeur, CV_32S);
	ImageInfoCV	imCtr ;
	im->copyTo(imCtr);
	findContours(imCtr, *(im->contours),*im->arbreContour, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[im1->channels()];
	if (im->statComposante==NULL)
		{
		im->statComposante = new cv::Mat*[im1->channels()]; 
		im->centreGComposante = new cv::Mat*[im1->channels()]; 
		im->contours = new std::vector<std::vector<cv::Point> >[im1->channels()]; 
		im->arbreContour = new std::vector<cv::Vec4i> [im1->channels()]; 
		for (int i=0;i<im1->channels();i++)
			{
			im->statComposante[i] = new cv::Mat; 
			im->centreGComposante[i] = new cv::Mat; 
			}
		}
		
	cv::split( *im1, planCouleur );
	for (int i=0;i<im1->channels();i++)
		{
		connectedComponentsWithStats(planCouleur[i], d[i],*(im->statComposante[i]),*(im->centreGComposante[i]), paramOCV.intParam["connectivity"].valeur, CV_32S);
		ImageInfoCV	imCtr ;
		d[i].copyTo(imCtr);
		findContours(imCtr, im->contours[i],im->arbreContour[i], cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
		}
	cv::merge((const cv::Mat *)d, im1->channels(), *im);
	delete []d;
	}
return im;
}


ImageInfoCV *ImageInfoCV::PartageEaux (ImageInfoCV	*im1,ImageInfoCV	*im2,Parametre *pOCV)

{
ImageInfoCV	*im =new ImageInfoCV;
if (!im2)
	return im;
im2->convertTo(*im, CV_32S);
watershed(*im1, *im);
return im;
}

ImageInfoCV		*ImageInfoCV::DistanceDiscrete (ImageInfoCV *im1,Parametre &paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (im1->channels()==1)
	{
	cv::distanceTransform(*im1, *im, cv::DIST_L2, 3);
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[im1->channels()];
		
	cv::split( *im1, planCouleur );
	for (int i=0;i<im1->channels();i++)
		{
		cv::distanceTransform(planCouleur[i], d[i], cv::DIST_L2, 3);
		}
	cv::merge((const cv::Mat *)d, im1->channels(), *im);
	delete []d;
	}
return im;
}


ImageInfoCV		*ImageInfoCV::Voronoi (ImageInfoCV *imSrc,Parametre &paramOCV)
{
ImageInfoCV	*im =new ImageInfoCV;
if (imSrc->channels()==1)
	{

	connectedComponentsWithStats(*imSrc, *im,*(im->statComposante[0]),*(im->centreGComposante[0]), paramOCV.intParam["connectivity"].valeur, CV_32S);
	ImageInfoCV	imCtr ;
	im->copyTo(imCtr);
	findContours(imCtr, *(im->contours),*im->arbreContour, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
	}
else
	{
	std::vector<Mat> planCouleur;
	Mat *d=new Mat[imSrc->channels()];
		
	cv::split( *imSrc, planCouleur );
	for (int i=0;i<imSrc->channels();i++)
		{
		connectedComponentsWithStats(planCouleur[i], d[i],*(im->statComposante[i]),*(im->centreGComposante[i]), paramOCV.intParam["connectivity"].valeur, CV_32S);
		ImageInfoCV	imCtr ;
		d[i].copyTo(imCtr);
		findContours(imCtr, im->contours[i],im->arbreContour[i], cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
		}
	cv::merge((const cv::Mat *)d, imSrc->channels(), *im);
	delete []d;
	}
return im;
}