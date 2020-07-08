// ECOMathDoc.cpp : implementation of the CECOMathDoc class
//

#include "stdafx.h"
#include "ECOMath.h"

#include "ECOMathDoc.h"
#include "SourceView.h"

#include "registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECOMathDoc

IMPLEMENT_DYNCREATE(CECOMathDoc, CDocument)

BEGIN_MESSAGE_MAP(CECOMathDoc, CDocument)
	//{{AFX_MSG_MAP(CECOMathDoc)
	ON_COMMAND(ID_ALPHA, OnAlpha)
	ON_COMMAND(ID_BETA, OnBeta)
	ON_COMMAND(ID_GAMMA, OnGamma)
	ON_COMMAND(ID_DELTA, OnDelta)
	ON_COMMAND(ID_EPSILON, OnEpsilon)
	ON_COMMAND(ID_ZETA, OnZeta)
	ON_COMMAND(ID_ETA, OnEta)
	ON_COMMAND(ID_THETA, OnTheta)
	ON_COMMAND(ID_IOTA, OnIota)
	ON_COMMAND(ID_KAPPA, OnKappa)
	ON_COMMAND(ID_LAMBDA, OnLambda)
	ON_COMMAND(ID_MU, OnMu)
	ON_COMMAND(ID_NU, OnNu)
	ON_COMMAND(ID_XI, OnXi)
	ON_COMMAND(ID_PI, OnPi)
	ON_COMMAND(ID_RHO, OnRho)
	ON_COMMAND(ID_SIGMA, OnSigma)
	ON_COMMAND(ID_TAU, OnTau)
	ON_COMMAND(ID_UPSILON, OnUpsilon)
	ON_COMMAND(ID_PHI, OnPhi)
	ON_COMMAND(ID_CHI, OnChi)
	ON_COMMAND(ID_PSI, OnPsi)
	ON_COMMAND(ID_OMEGA, OnOmega)
	ON_COMMAND(ID_OMICRON, OnOmicron)
	ON_COMMAND(ID_UPGAMMA, OnUpgamma)
	ON_COMMAND(ID_UPDELTA, OnUpdelta)
	ON_COMMAND(ID_UPTHETA, OnUptheta)
	ON_COMMAND(ID_UPXI, OnUpxi)
	ON_COMMAND(ID_UPLAMBDA, OnUplambda)
	ON_COMMAND(ID_UPPI, OnUppi)
	ON_COMMAND(ID_UPSIGMA, OnUpsigma)
	ON_COMMAND(ID_UPUPSILON, OnUpupsilon)
	ON_COMMAND(ID_UPPHI, OnUpphi)
	ON_COMMAND(ID_UPPSI, OnUppsi)
	ON_COMMAND(ID_UPOMEGA, OnUpomega)
	ON_COMMAND(ID_LT, OnLt)
	ON_COMMAND(ID_GT, OnGt)
	ON_COMMAND(ID_EQ, OnEq)
	ON_COMMAND(ID_LE, OnLe)
	ON_COMMAND(ID_GE, OnGe)
	ON_COMMAND(ID_PREC, OnPrec)
	ON_COMMAND(ID_SUCC, OnSucc)
	ON_COMMAND(ID_TRIANGLELEFT, OnTriangleleft)
	ON_COMMAND(ID_TRIANGLERIGHT, OnTriangleright)
	ON_COMMAND(ID_NE, OnNe)
	ON_COMMAND(ID_EQUIV, OnEquiv)
	ON_COMMAND(ID_APPROX, OnApprox)
	ON_COMMAND(ID_CONG, OnCong)
	ON_COMMAND(ID_PROPTO, OnPropto)
	ON_COMMAND(ID_DIV, OnDiv)
	ON_COMMAND(ID_PLUS, OnPlus)
	ON_COMMAND(ID_MINUS, OnMinus)
	ON_COMMAND(ID_PM, OnPm)
	ON_COMMAND(ID_MP, OnMp)
	ON_COMMAND(ID_TIMES, OnTimes)
	ON_COMMAND(ID_AST, OnAst)
	ON_COMMAND(ID_CDOTS, OnCdots)
	ON_COMMAND(ID_CIRC, OnCirc)
	ON_COMMAND(ID_BULLET, OnBullet)
	ON_COMMAND(ID_OTIMES, OnOtimes)
	ON_COMMAND(ID_OPLUS, OnOplus)
	ON_COMMAND(ID_LANGLE, OnLangle)
	ON_COMMAND(ID_RANGLE, OnRangle)
	ON_COMMAND(ID_LEFTARROW, OnLeftarrow)
	ON_COMMAND(ID_RIGHTARROW, OnRightarrow)
	ON_COMMAND(ID_LEFTRIGHTARROW, OnLeftrightarrow)
	ON_COMMAND(ID_UPARROW, OnUparrow)
	ON_COMMAND(ID_DOWNARROW, OnDownarrow)
	ON_COMMAND(ID_UPDOWNARROW, OnUpdownarrow)
	ON_COMMAND(ID_LLEFTARROW, OnLleftarrow)
	ON_COMMAND(ID_RRIGHTARROW, OnRrightarrow)
	ON_COMMAND(ID_LLEFTRIGHTARROW, OnLleftrightarrow)
	ON_COMMAND(ID_UUPARROW, OnUuparrow)
	ON_COMMAND(ID_DDOWNARROW, OnDdownarrow)
	ON_COMMAND(ID_UUPDOWNARROW, OnUupdownarrow)
	ON_COMMAND(ID_MAPSTO, OnMapsto)
	ON_COMMAND(ID_THEREFORE, OnTherefore)
	ON_COMMAND(ID_BECAUSE, OnBecause)
	ON_COMMAND(ID_EXISTS, OnExists)
	ON_COMMAND(ID_FORALL, OnForall)
	ON_COMMAND(ID_NEG, OnNeg)
	ON_COMMAND(ID_EDGE, OnEdge)
	ON_COMMAND(ID_VEE, OnVee)
	ON_COMMAND(ID_IN, OnIn)
	ON_COMMAND(ID_OWNS, OnOwns)
	ON_COMMAND(ID_CUP, OnCup)
	ON_COMMAND(ID_CAP, OnCap)
	ON_COMMAND(ID_SUBSET, OnSubset)
	ON_COMMAND(ID_SUPSET, OnSupset)
	ON_COMMAND(ID_SUBSETEQ, OnSubseteq)
	ON_COMMAND(ID_SUPSETEQ, OnSupseteq)
	ON_COMMAND(ID_NOTSUBSET, OnNotsubset)
	ON_COMMAND(ID_EMPTYSET, OnEmptyset)
	ON_COMMAND(ID_PARTIAL, OnPartial)
	ON_COMMAND(ID_NABLA, OnNabla)
	ON_COMMAND(ID_IM, OnIm)
	ON_COMMAND(ID_RE, OnRe)
	ON_COMMAND(ID_ALEPH, OnAleph)
	ON_COMMAND(ID_ANGLE, OnAngle)
	ON_COMMAND(ID_BOT, OnBot)
	ON_COMMAND(ID_DIAMONDSUIT, OnDiamondsuit)
	ON_COMMAND(ID_ELL, OnEll)
	ON_COMMAND(ID_WP, OnWp)
	ON_COMMAND(ID_UPCIRC, OnUpcirc)
	ON_COMMAND(ID_HBAR, OnHbar)
	ON_COMMAND(ID_SMALLINT, OnSmallint)
	ON_COMMAND(ID_SUM, OnSum)
	ON_COMMAND(ID_PROD, OnProd)
	ON_COMMAND(ID_COPROD, OnCoprod)
	ON_COMMAND(ID_ROUND_BRACKET, OnRoundBracket)
	ON_COMMAND(ID_SQUARE_BRACKET, OnSquareBracket)
	ON_COMMAND(ID_CURLY_BRACKET, OnCurlyBracket)
	ON_COMMAND(ID_ANGLE_BRACKET, OnAngleBracket)
	ON_COMMAND(ID_SINGLE_VERTICAL_BAR, OnSingleVerticalBar)
	ON_COMMAND(ID_DOUBLE_VERTICAL_BAR, OnDoubleVerticalBar)
	ON_COMMAND(ID_FLOOR_BRACKET, OnFloorBracket)
	ON_COMMAND(ID_CEILING_BRACKET, OnCeilingBracket)
	ON_COMMAND(ID_UPPER_HORIZENTAL_BRACE, OnUpperHorizentalBrace)
	ON_COMMAND(ID_LOWER_HORIZENTAL_BRACE, OnLowerHorizentalBrace)
	ON_COMMAND(ID_LEFT_PARENTHESIS, OnLeftParenthesis)
	ON_COMMAND(ID_RIGHT_PARENTHESIS, OnRightParenthesis)
	ON_COMMAND(ID_LEFT_BRACKET, OnLeftBracket)
	ON_COMMAND(ID_RIGHT_BRACKET, OnRightBracket)
	ON_COMMAND(ID_LEFT_BRACE, OnLeftBrace)
	ON_COMMAND(ID_RIGHT_BRACE, OnRightBrace)
	ON_COMMAND(ID_LEFT_ANGLE_BRACKET, OnLeftAngleBracket)
	ON_COMMAND(ID_RIGHT_ANGLE_BRACKET, OnRightAngleBracket)
	ON_COMMAND(ID_LEFT_VERTICAL_BAR, OnLeftVerticalBar)
	ON_COMMAND(ID_RIGHT_VERTICAL_BAR, OnRightVerticalBar)
	ON_COMMAND(ID_LEFT_DOUBLE_BAR, OnLeftDoubleBar)
	ON_COMMAND(ID_RIGHT_DOUBLE_BAR, OnRightDoubleBar)
	ON_COMMAND(ID_FRACTION, OnFraction)
	ON_COMMAND(ID_DIAGONAL_FRACTION, OnDiagonalFraction)
	ON_COMMAND(ID_SLASH_FRACTION, OnSlashFraction)
	ON_COMMAND(ID_SQUARD_ROOT, OnSquardRoot)
	ON_COMMAND(ID_NTH_ROOT, OnNthRoot)
	ON_COMMAND(ID_LONG_DIVISION, OnLongDivision)
	ON_COMMAND(ID_SUM_NOLIMIT, OnSumNolimit)
	ON_COMMAND(ID_SUM_UNDER, OnSumUnder)
	ON_COMMAND(ID_SUM_UNDEROVER, OnSumUnderover)
	ON_COMMAND(ID_SUM_SUB, OnSumSub)
	ON_COMMAND(ID_SUM_SUBSUPER, OnSumSubsuper)
	ON_COMMAND(ID_INDEFINITE_INTEGRAL, OnIndefiniteIntegral)
	ON_COMMAND(ID_INTEGRAL_OVERUNDER, OnIntegralOverunder)
	ON_COMMAND(ID_INTEGRAL_SUBSUPER, OnIntegralSubsuper)
	ON_COMMAND(ID_SUPERSCRIPT, OnSuperscript)
	ON_COMMAND(ID_SUBSCRIPT, OnSubscript)
	ON_COMMAND(ID_SUPER_SUBSCRIPT, OnSuperSubscript)
	ON_COMMAND(ID_LEFT_SUPERSCRIPT, OnLeftSuperscript)
	ON_COMMAND(ID_LEFT_SUBSCRIPT, OnLeftSubscript)
	ON_COMMAND(ID_LEFT_SUPER_SUBSCRIPT, OnLeftSuperSubscript)
	ON_COMMAND(ID_OVER_SCRIPT, OnOverScript)
	ON_COMMAND(ID_UNDER_SCRIPT, OnUnderScript)
	ON_COMMAND(ID_OVER_UNDER_SCRIPT, OnOverUnderScript)
	ON_COMMAND(ID_OVER_BAR, OnOverBar)
	ON_COMMAND(ID_UNDER_BAR, OnUnderBar)
	ON_COMMAND(ID_RIGHTOVER_BAR, OnRightoverBar)
	ON_COMMAND(ID_LEFTOVER_BAR, OnLeftoverBar)
	ON_COMMAND(ID_RIGHTUNVER_BAR, OnRightunverBar)
	ON_COMMAND(ID_LEFTUNDER_BAR, OnLeftunderBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECOMathDoc construction/destruction

CECOMathDoc::CECOMathDoc()
{
	// TODO: add one-time construction code here
	CBlankObject *blank = new CBlankObject();
	blank ->SetPosition(100, 100);
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);
}

CECOMathDoc::~CECOMathDoc()
{

}

BOOL CECOMathDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	POSITION pd;

	pd = GetFirstViewPosition();

	CView *pView = GetNextView(pd);
	pView = GetNextView(pd);
	pView ->EnableWindow(FALSE);

	//////////////////////////////////
	// Delete m_Math
	m_Math.DeleteAllElement();
	
	CBlankObject *blank = new CBlankObject();
	blank ->SetPosition(100, 100);
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	ReDrawAllView();

	pd = GetFirstViewPosition();

	pView = GetNextView(pd);
/*
	try {
		CRegistry regConfig;
		DWORD dwSomeDWData;

		dwSomeDWData = (DWORD)(pView ->m_hWnd);
		regConfig.Open( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\(ÁÖ)ECO\\EcoMath\\") );

		regConfig.Write(_T("ID"), (CONST BYTE *)&dwSomeDWData, sizeof(DWORD), REG_DWORD);

		regConfig.Close();
	}
	catch( CRegistryException *regError ) {
		// display the error
		regError->ReportError();

		// now delete the handle
		regError->Delete();

		return false;
	}
*/
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CECOMathDoc serialization

VOID CECOMathDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CECOMathDoc diagnostics

#ifdef _DEBUG
VOID CECOMathDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CECOMathDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CECOMathDoc commands

VOID CECOMathDoc::OnAlpha() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('a');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnBeta() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('b');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnGamma() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('g');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnDelta() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('d');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnEpsilon() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('e');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnZeta() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('z');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnEta() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('h');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnTheta() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('q');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnIota() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('i');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnKappa() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('k');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnLambda() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('l');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnMu() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('m');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnNu() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('n');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnOmicron() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('o');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnXi() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('x');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnPi() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('p');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnRho() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('r');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnSigma() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('s');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnTau() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('t');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnUpsilon() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('u');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnPhi() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('f');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnChi() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('c');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnPsi() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('y');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnOmega() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('w');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}



VOID CECOMathDoc::ClickASCII(TCHAR _ch)
{
	CASCIIObject *ch = new CASCIIObject();
	ch ->SetChar(_ch);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnUpgamma() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('G');

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnUpdelta() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('D');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnUptheta() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('Q');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnUplambda() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('L');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnUpxi() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('X');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}


VOID CECOMathDoc::OnUppi() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('P');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnUpsigma() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('S');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnUpupsilon() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('U');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnUpphi() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('F');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnUppsi() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('Y');

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnUpomega() 
{
	// TODO: Add your command handler code here
	CGreekObject *ch = new CGreekObject();
	ch ->SetChar('W');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::LeftButtonProcess()
{
	m_Math.MoveLeft();

	ReDrawAllView();
}

VOID CECOMathDoc::RightButtonProcess()
{
	m_Math.MoveRight();

	ReDrawAllView();
}

VOID CECOMathDoc::UpButtonProcess()
{
	m_Math.MoveUp();

	ReDrawAllView();
}

VOID CECOMathDoc::DownButtonProcess()
{
	m_Math.MoveDown();

	ReDrawAllView();
}

VOID CECOMathDoc::HomeButtonProcess()
{
	m_Math.MoveHome();

	ReDrawAllView();
}

VOID CECOMathDoc::EndButtonProcess()
{
	m_Math.MoveEnd();

	ReDrawAllView();
}

VOID CECOMathDoc::BackButtonProcess()
{
	m_Math.DeletePrev();

	ReDrawAllView();
}

VOID CECOMathDoc::DeleteButtonProcess()
{
	m_Math.DeleteNext();

	ReDrawAllView();
}

VOID CECOMathDoc::ReDrawAllView()
{
	POSITION pd;

	pd = GetFirstViewPosition();

	CView *pView = GetNextView(pd);
	pView ->Invalidate();	// MathView Redraw
	pView ->Invalidate();	// MathView Redraw

	pView = GetNextView(pd);
	((CSourceView *)pView) -> UpdateView();		// SourceView Redraw
}

VOID CECOMathDoc::OnLt() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x3C);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnEq() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x3D);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnGt() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x3E);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnLe() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x2266);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnGe() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x2267);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnPrec() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x227A);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnSucc() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x227B);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnTriangleleft() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x22B3);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnTriangleright() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x22B2);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnNe() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x2260);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnEquiv() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x2261);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnApprox() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x2248);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnCong() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x2245);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnPropto() 
{
	// TODO: Add your command handler code here
	CRelationObject *ch = new CRelationObject();
	ch ->SetChar(0x221D);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnPlus() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0x2B);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnMinus() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0x2D);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnPm() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0xB1);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnMp() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0x2213);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnTimes() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0xD7);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnDiv() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0xF7);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnAst() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0x2217);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnCdots() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0x2E);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();			
}

VOID CECOMathDoc::OnCirc() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0x2218);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();			
}

VOID CECOMathDoc::OnBullet() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0x2219);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();				
}

VOID CECOMathDoc::OnOtimes() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0x2297);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();				
}

VOID CECOMathDoc::OnOplus() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0x2295);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnLangle() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(0);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnRangle() 
{
	// TODO: Add your command handler code here
	CBiOpObject *ch = new CBiOpObject();
	ch ->SetChar(1);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnLeftarrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x2192);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnRightarrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x2190);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnLeftrightarrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x2194);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnUparrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x2191);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnDownarrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x2193);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnUpdownarrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x2195);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnLleftarrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x21D2);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnRrightarrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x21D0);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnLleftrightarrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x21D4);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnUuparrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x21D1);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnDdownarrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x21D3);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnUupdownarrow() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x21D5);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnMapsto() 
{
	// TODO: Add your command handler code here
	CArrowObject *ch = new CArrowObject();
	ch ->SetChar(0x21A6);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnTherefore() 
{
	// TODO: Add your command handler code here
	CLogicalObject *ch = new CLogicalObject();
	ch ->SetChar(0x2234);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnBecause() 
{
	// TODO: Add your command handler code here
	CLogicalObject *ch = new CLogicalObject();
	ch ->SetChar(0x2235);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnExists() 
{
	// TODO: Add your command handler code here
	CLogicalObject *ch = new CLogicalObject();
	ch ->SetChar(0x2203);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnForall() 
{
	// TODO: Add your command handler code here
	CLogicalObject *ch = new CLogicalObject();
	ch ->SetChar(0x2200);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnNeg() 
{
	// TODO: Add your command handler code here
	CLogicalObject *ch = new CLogicalObject();
	ch ->SetChar(0x2510);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnEdge() 
{
	// TODO: Add your command handler code here
	CLogicalObject *ch = new CLogicalObject();
	ch ->SetChar(0x2227);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnVee() 
{
	// TODO: Add your command handler code here
	CLogicalObject *ch = new CLogicalObject();
	ch ->SetChar(0x2228);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnIn() 
{
	// TODO: Add your command handler code here
	CSetObject *ch = new CSetObject();
	ch ->SetChar(0x2208);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnOwns() 
{
	// TODO: Add your command handler code here
	CSetObject *ch = new CSetObject();
	ch ->SetChar(0x220B);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnCup() 
{
	// TODO: Add your command handler code here
	CSetObject *ch = new CSetObject();
	ch ->SetChar(0x222A);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnCap() 
{
	// TODO: Add your command handler code here
	CSetObject *ch = new CSetObject();
	ch ->SetChar(0x2229);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();
}

VOID CECOMathDoc::OnSubset() 
{
	// TODO: Add your command handler code here
	CSetObject *ch = new CSetObject();
	ch ->SetChar(0x2282);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnSupset() 
{
	// TODO: Add your command handler code here
	CSetObject *ch = new CSetObject();
	ch ->SetChar(0x2283);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnSubseteq() 
{
	// TODO: Add your command handler code here
	CSetObject *ch = new CSetObject();
	ch ->SetChar(0x2286);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnSupseteq() 
{
	// TODO: Add your command handler code here
	CSetObject *ch = new CSetObject();
	ch ->SetChar(0x2287);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnNotsubset() 
{
	// TODO: Add your command handler code here
	CSetObject *ch = new CSetObject();
	ch ->SetChar(0x2284);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnEmptyset() 
{
	// TODO: Add your command handler code here
	CSetObject *ch = new CSetObject();
	ch ->SetChar(0x2205);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnPartial() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x2202);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnNabla() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x2207);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnIm() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x2111);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnRe() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x211B);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnAleph() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar('!');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnAngle() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x2220);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnBot() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x22A5);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnDiamondsuit() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x22CA);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnEll() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar('!');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnWp() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar('!');
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnUpcirc() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x00BA);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnHbar() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x210F);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnSmallint() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x222B);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnSum() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x2211);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnProd() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x220F);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnCoprod() 
{
	// TODO: Add your command handler code here
	CMiscObject *ch = new CMiscObject();
	ch ->SetChar(0x2210);
	ch ->SetVisible(true);

	m_Math.InsertTexObject(ch);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnRoundBracket() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(0);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(0);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();
}

VOID CECOMathDoc::OnSquareBracket() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(1);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(1);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnCurlyBracket() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(2);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(2);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnAngleBracket() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(3);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(3);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnSingleVerticalBar() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(4);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(4);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnDoubleVerticalBar() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(5);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(5);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnFloorBracket() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(6);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(6);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnCeilingBracket() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(7);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(7);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnUpperHorizentalBrace() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(8);
	fenceLeft ->SetIsLeft(true);
	fenceLeft ->SetSize(10, 10);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *lbrace1 = new CBlankObject();
	lbrace1 ->SetSize(20, 20);
	lbrace1 ->SetChar('{');
	lbrace1 ->SetVisible(false);
	m_Math.InsertTexObject(lbrace1);

	CBlankObject *blank1 = new CBlankObject();
	blank1 ->SetSize(20, 20);
	m_Math.InsertTexObject(blank1);

	CBlankObject *rbrace1 = new CBlankObject();
	rbrace1 ->SetSize(20, 20);
	rbrace1 ->SetChar('}');
	rbrace1 ->SetVisible(false);
	m_Math.InsertTexObject(rbrace1);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(8);
	fenceRight ->SetIsLeft(false);
	fenceRight ->SetSize(10, 10);
	m_Math.InsertTexObject(fenceRight);

	CBlankObject *lbrace2 = new CBlankObject();
	lbrace2 ->SetSize(20, 20);
	lbrace2 ->SetChar('{');
	lbrace2 ->SetVisible(false);
	m_Math.InsertTexObject(lbrace2);

	CBlankObject *blank2 = new CBlankObject();
	blank2 ->SetSize(20, 20);
	m_Math.InsertTexObject(blank2);

	CBlankObject *rbrace2 = new CBlankObject();
	rbrace2 ->SetSize(20, 20);
	rbrace2 ->SetChar('}');
	rbrace2 ->SetVisible(false);
	m_Math.InsertTexObject(rbrace2);


	fenceLeft -> m_pLeftBrace = lbrace1;
	fenceLeft -> m_pRightBrace = rbrace1;
	fenceLeft -> m_pRightFence = fenceRight;

	fenceRight -> m_pLeftBrace = lbrace2;
	fenceRight -> m_pRightBrace = rbrace2;

	ReDrawAllView();		
}

VOID CECOMathDoc::OnLowerHorizentalBrace() 
{
	// TODO: Add your command handler code here

	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(9);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *lbrace1 = new CBlankObject();
	lbrace1 ->SetSize(20, 20);
	lbrace1 ->SetChar('{');
	lbrace1 ->SetVisible(false);
	m_Math.InsertTexObject(lbrace1);

	CBlankObject *blank1 = new CBlankObject();
	blank1 ->SetSize(20, 20);
	m_Math.InsertTexObject(blank1);

	CBlankObject *rbrace1 = new CBlankObject();
	rbrace1 ->SetSize(20, 20);
	rbrace1 ->SetChar('}');
	rbrace1 ->SetVisible(false);
	m_Math.InsertTexObject(rbrace1);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(9);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	CBlankObject *lbrace2 = new CBlankObject();
	lbrace2 ->SetSize(20, 20);
	lbrace2 ->SetChar('{');
	lbrace2 ->SetVisible(false);
	m_Math.InsertTexObject(lbrace2);

	CBlankObject *blank2 = new CBlankObject();
	blank2 ->SetSize(20, 20);
	m_Math.InsertTexObject(blank2);

	CBlankObject *rbrace2 = new CBlankObject();
	rbrace2 ->SetSize(20, 20);
	rbrace2 ->SetChar('}');
	rbrace2 ->SetVisible(false);
	m_Math.InsertTexObject(rbrace2);


	fenceLeft -> m_pLeftBrace = lbrace1;
	fenceLeft -> m_pRightBrace = rbrace1;
	fenceLeft -> m_pRightFence = fenceRight;

	fenceRight -> m_pLeftBrace = lbrace2;
	fenceRight -> m_pRightBrace = rbrace2;

	ReDrawAllView();		
}

VOID CECOMathDoc::OnLeftParenthesis() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(10);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(10);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnRightParenthesis() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(11);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(11);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnLeftBracket() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(12);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(12);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnRightBracket() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(13);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(13);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnLeftBrace() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(14);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(14);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();
}

VOID CECOMathDoc::OnRightBrace() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(15);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(15);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnLeftAngleBracket() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(16);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(16);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnRightAngleBracket() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(17);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(17);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnLeftVerticalBar() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(18);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(18);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnRightVerticalBar() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(19);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(19);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnLeftDoubleBar() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(20);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(20);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnRightDoubleBar() 
{
	// TODO: Add your command handler code here
	CFenceObject *fenceLeft = new CFenceObject();
	fenceLeft ->SetType(21);
	fenceLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fenceLeft);

	CBlankObject *blank = new CBlankObject();
	blank ->SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CFenceObject *fenceRight = new CFenceObject();
	fenceRight ->SetType(21);
	fenceRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fenceRight);

	ReDrawAllView();		
}

//////////////////////////////////////////////
//
// ºÐ¼ö
//
VOID CECOMathDoc::OnFraction() 
{
	// TODO: Add your command handler code here
	CFractionObject *fractionLeft = new CFractionObject();
	fractionLeft ->SetType(0);
	fractionLeft ->SetIsMiddle(false);
	fractionLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fractionLeft);

	CBlankObject *lbrace1 = new CBlankObject();
	lbrace1 ->SetSize(20, 20);
	lbrace1 ->SetChar('{');
	lbrace1 ->SetVisible(false);
	m_Math.InsertTexObject(lbrace1);

	CBlankObject *blankLeft = new CBlankObject();
	blankLeft ->SetSize(20, 20);
	m_Math.InsertTexObject(blankLeft);

	CBlankObject *rbrace1 = new CBlankObject();
	rbrace1 ->SetSize(20, 20);
	rbrace1 ->SetChar('}');
	rbrace1 ->SetVisible(false);
	m_Math.InsertTexObject(rbrace1);

	CFractionObject *fractionMiddle = new CFractionObject();
	fractionMiddle ->SetType(0);
	fractionMiddle ->SetIsMiddle(true);
	fractionMiddle ->SetIsLeft(false);
	fractionMiddle ->SetVisible(false);
	m_Math.InsertTexObject(fractionMiddle);

	CBlankObject *lbrace2 = new CBlankObject();
	lbrace2 ->SetSize(20, 20);
	lbrace2 ->SetChar('{');
	lbrace2 ->SetVisible(false);
	m_Math.InsertTexObject(lbrace2);

	CBlankObject *blankRight = new CBlankObject();
	blankRight ->SetSize(20, 20);
	m_Math.InsertTexObject(blankRight);

	CBlankObject *rbrace2 = new CBlankObject();
	rbrace2 ->SetSize(20, 20);
	rbrace2 ->SetChar('}');
	rbrace2 ->SetVisible(false);
	m_Math.InsertTexObject(rbrace2);

	CFractionObject *fractionRight = new CFractionObject();
	fractionRight ->SetType(0);
	fractionRight ->SetIsMiddle(false);
	fractionRight ->SetIsLeft(false);
	fractionRight ->SetVisible(false);
	m_Math.InsertTexObject(fractionRight);

	ReDrawAllView();			
}

VOID CECOMathDoc::OnDiagonalFraction() 
{
	// TODO: Add your command handler code here
	CFractionObject *fractionLeft = new CFractionObject();
	fractionLeft ->SetType(1);
	fractionLeft ->SetIsMiddle(false);
	fractionLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fractionLeft);

	CBlankObject *blankLeft = new CBlankObject();
	blankLeft ->SetSize(20, 20);
	m_Math.InsertTexObject(blankLeft);

	CFractionObject *fractionMiddle = new CFractionObject();
	fractionMiddle ->SetType(1);
	fractionMiddle ->SetIsMiddle(true);
	fractionMiddle ->SetIsLeft(false);
	m_Math.InsertTexObject(fractionMiddle);

	CBlankObject *blankRight = new CBlankObject();
	blankRight ->SetSize(20, 20);
	m_Math.InsertTexObject(blankRight);

	CFractionObject *fractionRight = new CFractionObject();
	fractionRight ->SetType(1);
	fractionRight ->SetIsMiddle(false);
	fractionRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fractionRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnSlashFraction() 
{
	// TODO: Add your command handler code here
	CFractionObject *fractionLeft = new CFractionObject();
	fractionLeft ->SetType(2);
	fractionLeft ->SetIsMiddle(false);
	fractionLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(fractionLeft);

	CBlankObject *blankLeft = new CBlankObject();
	blankLeft ->SetSize(20, 20);
	m_Math.InsertTexObject(blankLeft);

	CFractionObject *fractionMiddle = new CFractionObject();
	fractionMiddle ->SetType(2);
	fractionMiddle ->SetIsMiddle(true);
	fractionMiddle ->SetIsLeft(false);
	m_Math.InsertTexObject(fractionMiddle);

	CBlankObject *blankRight = new CBlankObject();
	blankRight ->SetSize(20, 20);
	m_Math.InsertTexObject(blankRight);

	CFractionObject *fractionRight = new CFractionObject();
	fractionRight ->SetType(2);
	fractionRight ->SetIsMiddle(false);
	fractionRight ->SetIsLeft(false);
	m_Math.InsertTexObject(fractionRight);

	ReDrawAllView();	
}

//////////////////////////////////////////////
//
// Root & Division
//
VOID CECOMathDoc::OnSquardRoot() 
{
	// TODO: Add your command handler code here
	CRadicalObject *radicalLeft = new CRadicalObject();
	radicalLeft ->SetType(0);
	radicalLeft ->SetIsLeft(true);
	radicalLeft ->SetSize(20, 20);
	m_Math.InsertTexObject(radicalLeft);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CRadicalObject *radicalRight = new CRadicalObject();
	radicalRight ->SetType(0);
	radicalRight ->SetIsLeft(false);
	radicalRight ->SetSize(20, 20);
	m_Math.InsertTexObject(radicalRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnNthRoot() 
{
	// TODO: Add your command handler code here
	CRadicalObject *radicalLeft = new CRadicalObject();
	radicalLeft ->SetType(1);
	radicalLeft ->SetIsLeft(true);
	radicalLeft ->SetSize(20, 20);
	m_Math.InsertTexObject(radicalLeft);

	CBlankObject *blankLeft = new CBlankObject();
	blankLeft -> SetSize(20, 20);
	m_Math.InsertTexObject(blankLeft);

	CRadicalObject *radicalMiddle = new CRadicalObject();
	radicalMiddle ->SetType(1);
	radicalMiddle ->SetIsMiddle(true);
	radicalMiddle ->SetSize(20, 20);
	m_Math.InsertTexObject(radicalMiddle);

	CBlankObject *blankRight = new CBlankObject();
	blankRight -> SetSize(20, 20);
	m_Math.InsertTexObject(blankRight);

	CRadicalObject *radicalRight = new CRadicalObject();
	radicalRight ->SetType(1);
	radicalRight ->SetIsLeft(false);
	radicalRight ->SetSize(20, 20);
	m_Math.InsertTexObject(radicalRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnLongDivision() 
{
	// TODO: Add your command handler code here
	CRadicalObject *radical = new CRadicalObject();
	radical ->SetType(2);
	m_Math.InsertTexObject(radical);

	ReDrawAllView();	
}

//////////////////////////////////////////////
//
// Script
//
VOID CECOMathDoc::OnSuperscript() 
{
	// TODO: Add your command handler code here
	CScriptObject *scriptLeft = new CScriptObject();
	scriptLeft ->SetType(0);
	scriptLeft ->SetIsLeft(true);
	scriptLeft ->SetSize(20, 20);
	m_Math.InsertTexObject(scriptLeft);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CScriptObject *scriptRight = new CScriptObject();
	scriptRight ->SetType(0);
	scriptRight ->SetIsLeft(false);
	scriptRight ->SetSize(20, 20);
	m_Math.InsertTexObject(scriptRight);

	ReDrawAllView();		
}


VOID CECOMathDoc::OnSubscript() 
{
	// TODO: Add your command handler code here
	CScriptObject *scriptLeft = new CScriptObject();
	scriptLeft ->SetType(1);
	scriptLeft ->SetIsLeft(true);
	scriptLeft ->SetSize(20, 20);
	m_Math.InsertTexObject(scriptLeft);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CScriptObject *scriptRight = new CScriptObject();
	scriptRight ->SetType(1);
	scriptRight ->SetIsLeft(false);
	scriptRight ->SetSize(20, 20);
	m_Math.InsertTexObject(scriptRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnSuperSubscript() 
{
	// TODO: Add your command handler code here
	CScriptObject *scriptLeft = new CScriptObject();
	scriptLeft ->SetType(2);
	scriptLeft ->SetIsLeft(true);
	scriptLeft ->SetSize(20, 20);
	m_Math.InsertTexObject(scriptLeft);

	CBlankObject *blankLeft = new CBlankObject();
	blankLeft -> SetSize(20, 20);
	m_Math.InsertTexObject(blankLeft);

	CScriptObject *scriptMiddle = new CScriptObject();
	scriptMiddle ->SetType(2);
	scriptMiddle ->SetIsMiddle(true);
	scriptMiddle ->SetSize(20, 20);
	m_Math.InsertTexObject(scriptMiddle);

	CBlankObject *blankRight = new CBlankObject();
	blankRight -> SetSize(20, 20);
	m_Math.InsertTexObject(blankRight);

	CScriptObject *scriptRight = new CScriptObject();
	scriptRight ->SetType(2);
	scriptRight ->SetIsLeft(false);
	scriptRight ->SetSize(20, 20);
	m_Math.InsertTexObject(scriptRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnLeftSuperscript() 
{
	// TODO: Add your command handler code here
	
}

VOID CECOMathDoc::OnLeftSubscript() 
{
	// TODO: Add your command handler code here
	
}

VOID CECOMathDoc::OnLeftSuperSubscript() 
{
	// TODO: Add your command handler code here
	
}

VOID CECOMathDoc::OnOverScript() 
{
	// TODO: Add your command handler code here
	
}

VOID CECOMathDoc::OnUnderScript() 
{
	// TODO: Add your command handler code here
	
}

VOID CECOMathDoc::OnOverUnderScript() 
{
	// TODO: Add your command handler code here
	
}


//////////////////////////////////////////////
//
// Sum
//
VOID CECOMathDoc::OnSumNolimit() 
{
	// TODO: Add your command handler code here
	CSumObject *sumLeft = new CSumObject();
	sumLeft ->SetType(0);
	sumLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(sumLeft);

	CBlankObject *lbrace1 = new CBlankObject();
	lbrace1 ->SetSize(20, 20);
	lbrace1 ->SetChar('{');
	lbrace1 ->SetVisible(false);
	m_Math.InsertTexObject(lbrace1);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CBlankObject *rbrace1 = new CBlankObject();
	rbrace1 ->SetSize(20, 20);
	rbrace1 ->SetChar('}');
	rbrace1 ->SetVisible(false);
	m_Math.InsertTexObject(rbrace1);

	CSumObject *sumRight = new CSumObject();
	sumRight ->SetType(0);
	sumRight ->SetIsLeft(false);
	m_Math.InsertTexObject(sumRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnSumUnder() 
{
	// TODO: Add your command handler code here
	CSumObject *sumLeft = new CSumObject();
	sumLeft ->SetType(1);
	sumLeft ->SetIsLeft(true);
	sumLeft ->SetIsMiddle(false);
	m_Math.InsertTexObject(sumLeft);

	CBlankObject *blankUnder = new CBlankObject();
	blankUnder -> SetSize(20, 20);
	m_Math.InsertTexObject(blankUnder);

	CSumObject *sumMiddle= new CSumObject();
	sumMiddle ->SetType(1);
	sumMiddle ->SetIsLeft(false);
	sumMiddle ->SetIsMiddle(true);
	m_Math.InsertTexObject(sumMiddle);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CSumObject *sumRight= new CSumObject();
	sumRight ->SetType(1);
	sumRight ->SetIsLeft(false);
	sumRight ->SetIsMiddle(false);
	m_Math.InsertTexObject(sumRight);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnSumUnderover() 
{
	// TODO: Add your command handler code here
	CSumObject *sumLeft = new CSumObject();
	sumLeft ->SetType(2);
	sumLeft ->SetIsLeft(true);
	sumLeft ->SetIsMiddle(false);
	m_Math.InsertTexObject(sumLeft);

	CBlankObject *blankUnder = new CBlankObject();
	blankUnder -> SetSize(20, 20);
	m_Math.InsertTexObject(blankUnder);

	CSumObject *sumLeftMiddle = new CSumObject();
	sumLeftMiddle ->SetType(2);
	sumLeftMiddle ->SetIsLeft(true);
	sumLeftMiddle ->SetIsMiddle(true);
	m_Math.InsertTexObject(sumLeftMiddle);

	CBlankObject *blankOver = new CBlankObject();
	blankOver -> SetSize(20, 20);
	m_Math.InsertTexObject(blankOver);

	CSumObject *sumMiddle = new CSumObject();
	sumMiddle ->SetType(2);
	sumMiddle ->SetIsLeft(false);
	sumMiddle ->SetIsMiddle(true);
	m_Math.InsertTexObject(sumMiddle);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CSumObject *sumRight = new CSumObject();
	sumRight ->SetType(2);
	sumRight ->SetIsLeft(false);
	sumRight ->SetIsMiddle(false);
	m_Math.InsertTexObject(sumRight);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnSumSub() 
{
	// TODO: Add your command handler code here
	CSumObject *sumLeft = new CSumObject();
	sumLeft ->SetType(3);
	sumLeft ->SetIsLeft(true);
	sumLeft ->SetIsMiddle(false);
	m_Math.InsertTexObject(sumLeft);

	CBlankObject *blankUnder = new CBlankObject();
	blankUnder -> SetSize(20, 20);
	m_Math.InsertTexObject(blankUnder);

	CSumObject *sumMiddle= new CSumObject();
	sumMiddle ->SetType(3);
	sumMiddle ->SetIsLeft(false);
	sumMiddle ->SetIsMiddle(true);
	m_Math.InsertTexObject(sumMiddle);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CSumObject *sumRight= new CSumObject();
	sumRight ->SetType(3);
	sumRight ->SetIsLeft(false);
	sumRight ->SetIsMiddle(false);
	m_Math.InsertTexObject(sumRight);

	ReDrawAllView();		
}

VOID CECOMathDoc::OnSumSubsuper() 
{
	// TODO: Add your command handler code here
	CSumObject *sumLeft = new CSumObject();
	sumLeft ->SetType(4);
	sumLeft ->SetIsLeft(true);
	sumLeft ->SetIsMiddle(false);
	m_Math.InsertTexObject(sumLeft);

	CBlankObject *blankUnder = new CBlankObject();
	blankUnder -> SetSize(20, 20);
	m_Math.InsertTexObject(blankUnder);

	CSumObject *sumLeftMiddle = new CSumObject();
	sumLeftMiddle ->SetType(4);
	sumLeftMiddle ->SetIsLeft(true);
	sumLeftMiddle ->SetIsMiddle(true);
	m_Math.InsertTexObject(sumLeftMiddle);

	CBlankObject *blankOver = new CBlankObject();
	blankOver -> SetSize(20, 20);
	m_Math.InsertTexObject(blankOver);

	CSumObject *sumMiddle = new CSumObject();
	sumMiddle ->SetType(4);
	sumMiddle ->SetIsLeft(false);
	sumMiddle ->SetIsMiddle(true);
	m_Math.InsertTexObject(sumMiddle);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CSumObject *sumRight = new CSumObject();
	sumRight ->SetType(4);
	sumRight ->SetIsLeft(false);
	sumRight ->SetIsMiddle(false);
	m_Math.InsertTexObject(sumRight);

	ReDrawAllView();		
}

//////////////////////////////////////////////
//
// Integral
//
VOID CECOMathDoc::OnIndefiniteIntegral() 
{
	// TODO: Add your command handler code here
	CIntegralObject *integralLeft = new CIntegralObject();
	integralLeft ->SetType(0);
	integralLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(integralLeft);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CIntegralObject *integralRight = new CIntegralObject();
	integralRight ->SetType(0);
	integralRight ->SetIsLeft(false);
	m_Math.InsertTexObject(integralRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnIntegralOverunder() 
{
	// TODO: Add your command handler code here
	CIntegralObject *integralLeft = new CIntegralObject();
	integralLeft ->SetType(1);
	integralLeft ->SetIsLeft(true);
	integralLeft ->SetIsMiddle(false);
	m_Math.InsertTexObject(integralLeft);

	CBlankObject *blankUnder = new CBlankObject();
	blankUnder -> SetSize(20, 20);
	m_Math.InsertTexObject(blankUnder);

	CIntegralObject *integralLeftMiddle = new CIntegralObject();
	integralLeftMiddle ->SetType(1);
	integralLeftMiddle ->SetIsLeft(true);
	integralLeftMiddle ->SetIsMiddle(true);
	m_Math.InsertTexObject(integralLeftMiddle);

	CBlankObject *blankOver = new CBlankObject();
	blankOver -> SetSize(20, 20);
	m_Math.InsertTexObject(blankOver);

	CIntegralObject *integralMiddle = new CIntegralObject();
	integralMiddle ->SetType(1);
	integralMiddle ->SetIsLeft(false);
	integralMiddle ->SetIsMiddle(true);
	m_Math.InsertTexObject(integralMiddle);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CIntegralObject *integralRight = new CIntegralObject();
	integralRight ->SetType(1);
	integralRight ->SetIsLeft(false);
	integralRight ->SetIsMiddle(false);
	m_Math.InsertTexObject(integralRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnIntegralSubsuper() 
{
	// TODO: Add your command handler code here
	CIntegralObject *integralLeft = new CIntegralObject();
	integralLeft ->SetType(2);
	integralLeft ->SetIsLeft(true);
	integralLeft ->SetIsMiddle(false);
	m_Math.InsertTexObject(integralLeft);

	CBlankObject *blankUnder = new CBlankObject();
	blankUnder -> SetSize(20, 20);
	m_Math.InsertTexObject(blankUnder);

	CIntegralObject *integralLeftMiddle = new CIntegralObject();
	integralLeftMiddle ->SetType(2);
	integralLeftMiddle ->SetIsLeft(true);
	integralLeftMiddle ->SetIsMiddle(true);
	m_Math.InsertTexObject(integralLeftMiddle);

	CBlankObject *blankOver = new CBlankObject();
	blankOver -> SetSize(20, 20);
	m_Math.InsertTexObject(blankOver);

	CIntegralObject *integralMiddle = new CIntegralObject();
	integralMiddle ->SetType(2);
	integralMiddle ->SetIsLeft(false);
	integralMiddle ->SetIsMiddle(true);
	m_Math.InsertTexObject(integralMiddle);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CIntegralObject *integralRight = new CIntegralObject();
	integralRight ->SetType(2);
	integralRight ->SetIsLeft(false);
	integralRight ->SetIsMiddle(false);
	m_Math.InsertTexObject(integralRight);

	ReDrawAllView();	
}

//////////////////////////////////////////////
//
// Underbar & Overbar
//
VOID CECOMathDoc::OnOverBar() 
{
	// TODO: Add your command handler code here
	CBarObject *barLeft = new CBarObject();
	barLeft ->SetType(0);
	barLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(barLeft);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CBarObject *barRight = new CBarObject();
	barRight ->SetType(0);
	barRight ->SetIsLeft(false);
	m_Math.InsertTexObject(barRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnUnderBar() 
{
	// TODO: Add your command handler code here
	CBarObject *barLeft = new CBarObject();
	barLeft ->SetType(1);
	barLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(barLeft);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CBarObject *barRight = new CBarObject();
	barRight ->SetType(1);
	barRight ->SetIsLeft(false);
	m_Math.InsertTexObject(barRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnRightoverBar() 
{
	// TODO: Add your command handler code here
	CBarObject *barLeft = new CBarObject();
	barLeft ->SetType(2);
	barLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(barLeft);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CBarObject *barRight = new CBarObject();
	barRight ->SetType(2);
	barRight ->SetIsLeft(false);
	m_Math.InsertTexObject(barRight);

	ReDrawAllView();	
}

VOID CECOMathDoc::OnLeftoverBar() 
{
	// TODO: Add your command handler code here
	CBarObject *barLeft = new CBarObject();
	barLeft ->SetType(3);
	barLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(barLeft);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CBarObject *barRight = new CBarObject();
	barRight ->SetType(3);
	barRight ->SetIsLeft(false);
	m_Math.InsertTexObject(barRight);	

	ReDrawAllView();
}

VOID CECOMathDoc::OnRightunverBar() 
{
	// TODO: Add your command handler code here
	CBarObject *barLeft = new CBarObject();
	barLeft ->SetType(4);
	barLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(barLeft);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CBarObject *barRight = new CBarObject();
	barRight ->SetType(4);
	barRight ->SetIsLeft(false);
	m_Math.InsertTexObject(barRight);	

	ReDrawAllView();	
}

VOID CECOMathDoc::OnLeftunderBar() 
{
	// TODO: Add your command handler code here
	CBarObject *barLeft = new CBarObject();
	barLeft ->SetType(5);
	barLeft ->SetIsLeft(true);
	m_Math.InsertTexObject(barLeft);

	CBlankObject *blank = new CBlankObject();
	blank -> SetSize(20, 20);
	m_Math.InsertTexObject(blank);

	CBarObject *barRight = new CBarObject();
	barRight ->SetType(5);
	barRight ->SetIsLeft(false);
	m_Math.InsertTexObject(barRight);	

	ReDrawAllView();	
}
