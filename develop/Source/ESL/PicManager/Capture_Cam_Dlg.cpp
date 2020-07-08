// Capture_Cam_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Capture_Cam_Dlg.h"
#include "CharConvert.h"
//#include "Select_Pic_Dlg.h"

#include "..\\..\\공통\\paintlib\common\plobject.h"
#include "..\\..\\공통\\paintlib\common\Filter\plfilterresizebilinear.h"
#include "..\\..\\공통\\paintlib\common\Filter\PLFilterCrop.h"
#include "..\\..\\공통\\paintlib\common\pljpegenc.h"
#include "..\\..\\공통\\paintlib\common\plbmpenc.h"

#define	IMAGEFROMFILE 0
#define IMAGEFROMCAMERA 1
#define PATHSIZE 256

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCapture_Cam_Dlg dialog


CCapture_Cam_Dlg::CCapture_Cam_Dlg(CWnd* pParent /*=NULL*/)
: CDialog(CCapture_Cam_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCapture_Cam_Dlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCapture_Cam_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCapture_Cam_Dlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCapture_Cam_Dlg, CDialog)
//{{AFX_MSG_MAP(CCapture_Cam_Dlg)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_BTN_CAMSETUP, OnBtnCamsetup)
ON_BN_CLICKED(IDC_BTN_CAPTURE, OnBtnCapture)
ON_WM_PAINT()
ON_BN_CLICKED(IDC_BTN_SELECT, OnBtnSelect)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCapture_Cam_Dlg message handlers

BOOL CCapture_Cam_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	this->isImageSelect = FALSE;
	
	if( !this->Initialize() )		// 캡쳐 장치 초기화
	{
		AfxMessageBox( _T("CAMERA 연결 실패!") );
		OnOK();
	}
	
	this->StartCapture();
	
	GetDlgItem(IDC_BTN_SELECT)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCapture_Cam_Dlg::Initialize()
{
	// 비디오 캡쳐 윈도우를 생성
	//m_hCapWnd = capCreateCaptureWindow( "Capture", WS_CHILD | WS_VISIBLE, 39, 41, 137, 142, this->m_hWnd, 0 );
	m_hCapWnd = capCreateCaptureWindow( _T("Capture"), WS_CHILD | WS_VISIBLE, 39, 41, 108, 144, this->m_hWnd, 0 );
	
	if( m_hCapWnd == NULL )
	{
		return FALSE;
	}
	
	capSetUserData( m_hCapWnd, this );
	
	// 캡쳐 윈도우와 장치 드라이버 연결 (장치 ID : 0)
	if( !capDriverConnect( m_hCapWnd, 0 ) )
	{
		m_hCapWnd = NULL;
		return FALSE;
	}
	
	// 캡쳐 정보 설정
	if( SetCaptureParam() == FALSE )
	{
		// 캡쳐 장치 드라이버 해제
		capDriverDisconnect( m_hCapWnd );
		return FALSE;
	}
	
	return TRUE;
}

void CCapture_Cam_Dlg::Finalize()
{
	// 캡쳐 프로세스 정지    
	capCaptureAbort( m_hCapWnd );        
	
	// 캡쳐 드라이버 연결 해제
	capDriverDisconnect( m_hCapWnd );
	
	m_hCapWnd = NULL;
}

BOOL CCapture_Cam_Dlg::SetCaptureParam()
{
	CAPTUREPARMS Parms;
	
	// 현재 캡쳐 정보 읽어옴
	capCaptureGetSetup( m_hCapWnd, &Parms, sizeof(Parms) );
	Parms.fAbortLeftMouse = FALSE;							// 캡쳐 중지를 위한 마우스 L버튼 사용 
	Parms.fAbortRightMouse = FALSE;							// 캡쳐 중지를 위한 마우스 R버튼 사용
	Parms.fYield = TRUE;									//쓰레드를 생성시켜 캡쳐링함
	Parms.fCaptureAudio = TRUE;								// 오디오 캡쳐 사용 여부
	Parms.wPercentDropForError = 10;						// 놓지는 프레임은 최대 10%로 제한
	Parms.dwRequestMicroSecPerFrame	= FPS_TO_MS(30);		// 영상 캡쳐 프레임 설정 Frame Per Second
	
	// 캡쳐 정보 설정
	if( !capCaptureSetSetup( m_hCapWnd, &Parms, sizeof(Parms) ) )
	{
		return FALSE;
	}
	
	// 현재 카메라 비디오 포맷 정보를 읽어옴 
	capGetVideoFormat( m_hCapWnd, &m_BmpInfo, sizeof(m_BmpInfo) );
	
	m_BmpInfo.bmiHeader.biBitCount	  =8;					 // 각 픽셀의 비트 수
	m_BmpInfo.bmiHeader.biCompression = 0;					 // 압축 방법(bi_rgb 또는 0: 무압축 비트맵)
	m_BmpInfo.bmiHeader.biWidth		  = 176;				 // 영상의 가로 크기
	m_BmpInfo.bmiHeader.biHeight	  = 144;				 // 영상의 세로 크기
	m_BmpInfo.bmiHeader.biSize		  = sizeof(BITMAPINFOHEADER); // BITMAPINFOHEADER 구조체의 크기
	
	// 카메라 비디오 포멧 설정
	capSetVideoFormat( m_hCapWnd, &m_BmpInfo, sizeof(m_BmpInfo) );
	
	return TRUE;	
}

BOOL CCapture_Cam_Dlg::StartCapture()
{
	// 현재 카메라 비디오 포맷 정보를 읽어옴 
	capGetVideoFormat( m_hCapWnd, &m_BmpInfo, sizeof(m_BmpInfo) );
	
	m_BmpInfo.bmiHeader.biBitCount	  =8;					 // 각 픽셀의 비트 수
	m_BmpInfo.bmiHeader.biCompression = 0;					 // 압축 방법(bi_rgb 또는 0: 무압축 비트맵)
	m_BmpInfo.bmiHeader.biWidth		  = 176;				 // 영상의 가로 크기
	m_BmpInfo.bmiHeader.biHeight	  = 144;				 // 영상의 세로 크기
	m_BmpInfo.bmiHeader.biSize		  = sizeof(BITMAPINFOHEADER); // BITMAPINFOHEADER 구조체의 크기
	
	// 카메라 비디오 포멧 설정
	capSetVideoFormat( m_hCapWnd, &m_BmpInfo, sizeof(m_BmpInfo) );

	capDlgVideoFormat(m_hCapWnd);
	
	capPreviewRate(m_hCapWnd, 60);
	capPreview(m_hCapWnd, TRUE);
	
	return TRUE;
}

void CCapture_Cam_Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	this->Finalize();
}

void CCapture_Cam_Dlg::OnBtnCamsetup() 
{
	// TODO: Add your control notification handler code here
	capDlgVideoSource(m_hCapWnd) ;
}

void CCapture_Cam_Dlg::OnBtnCapture()
{
	// TODO: Add your control notification handler code here	

	CString imageName;
	
	imageName.Format(_T("captureImage.bmp"));
	
	capGrabFrameNoStop(m_hCapWnd);					// 순간 캡쳐 
	
	
	TCHAR path[PATHSIZE] = {0};
	CString imagePath;
    GetCurrentDirectory(PATHSIZE, path);
	wcscat(path,_T("\\captureImage.bmp"));
	
	
	capFileSaveDIB(m_hCapWnd, path);   // 파일 저장
	
	this->isImageSelect = TRUE;
	
	SetFileName(path);

	m_pBmp = NULL;

	LoadBitmap("captureImage.bmp");

	this->Crop(0,0,108,144);

	this->InvalidateRect(NULL, TRUE);
	
	//선택 버튼 활성화
	GetDlgItem(IDC_BTN_SELECT)->EnableWindow(TRUE);

}


void CCapture_Cam_Dlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	if(this->isImageSelect)
	{
	
		this->m_pBmp->Draw(::GetDC(this->m_hWnd),100,250);

	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CCapture_Cam_Dlg::OnBtnSelect() 
{
	// TODO: Add your control notification handler code here
	this->SaveJpeg("captureImage.jpg");

	this->SetFileName(_T("captureImage.jpg"));

	this->OnOK();
}


void CCapture_Cam_Dlg::SetFileName(CString fileName)
{
	this->fileName = fileName;
}

CString CCapture_Cam_Dlg::GetFileName()
{
	return this->fileName;
}


INT CCapture_Cam_Dlg::Crop(INT XMin, INT YMin, INT XMax, INT YMax)
{
	PLFilterCrop Filter(XMin, YMin, XMax, YMax);	
	Filter.ApplyInPlace (m_pBmp);   	
	return 0;
}

void CCapture_Cam_Dlg::FreeBitmap(void)
{
	if (m_pBmp) {
		delete m_pBmp;
		m_pBmp = NULL;
	}

	return;
}

INT CCapture_Cam_Dlg::LoadBitmap(CString strFileName)
{
	FreeBitmap();
	m_pBmp = new PLWinBmp;
	if (m_pBmp == NULL) return -1;

	CHAR		*szFileName;	
	CharConvert cvt;
	INT			nLen;

	cvt.UnicodeToMultiByte(strFileName.GetBuffer(0), &szFileName, &nLen );

	m_Decoder = new PLAnyPicDecoder();
	m_Decoder->MakeBmpFromFile (szFileName, m_pBmp);

	delete m_Decoder;

	return 0;
}

INT CCapture_Cam_Dlg::SaveBitmap(CString strFileName)	// JPEG로 저장하므로 사용X
{
	if (m_pBmp == NULL) return -1;

	CHAR		*szFileName;	
	CharConvert cvt;
	INT			nLen;

	cvt.UnicodeToMultiByte(strFileName.GetBuffer(0), &szFileName, &nLen );

	PLBmpEncoder Encoder;
	Encoder.MakeFileFromBmp(szFileName, m_pBmp);

	return 0;
}

INT CCapture_Cam_Dlg::SaveJpeg(CString strFileName)
{
	if (m_pBmp == NULL) return -1;

	PLJPEGEncoder Encoder;

	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( strFileName.GetBuffer(0), &szTmp, &len );
	if( 0 < len)	
	{			
		Encoder.MakeFileFromBmp(szTmp, m_pBmp);
		delete [] szTmp;
	}
	
	return 0;
}