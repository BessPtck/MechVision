//#include "framework.h"
#include "DrawHexImg.h"

DrawHexImg::DrawHexImg() :m_hex(NULL), m_nodes(NULL), m_nHex(0), m_lowerHex(NULL), m_nLowerHex(0), m_hexMask(NULL), m_hexMaskPlus(NULL), m_lowerHexMask(NULL), m_hexedImg(NULL)
{
	m_defOCol.r= 0x00;
	m_defOCol.g = 0x00;
	m_defOCol.b = 0x00;
}
DrawHexImg::~DrawHexImg()
{
	;
}
unsigned char DrawHexImg::Init(
	long width,
	long height,
	long bpp,
	s_hex* hex,
	int nHex,
	Img* hexMask,
	Img* hexMaskPlus
) 
{
	m_hex = hex;
	m_nHex = nHex;
	m_hexMask = hexMask;
	m_hexMaskPlus = hexMaskPlus;
	m_hexedImg = new Img;
	if (m_hexedImg == NULL)
		return ECODE_MEMERR_FAIL;
	unsigned char err_code = m_hexedImg->init(width, height, bpp);
	return err_code;
}
unsigned char DrawHexImg::Init(HexBase* hBase)
{
	Img* img = hBase->getImg();
	return Init(img->getWidth(), img->getHeight(), img->getColorMode(), hBase->getHexes(), hBase->getNHex(), hBase->getHexMask(), hBase->getHexMaskPlus());
}
unsigned char DrawHexImg::Init(HexBase* hBase, HexBase* lowerHBase) {
	Img* img = hBase->getImg();
	m_lowerHexMask = lowerHBase->getHexMask();
	m_lowerHex = lowerHBase->getHexes();
	m_nLowerHex = lowerHBase->getNHex();
	return Init(img->getWidth(), img->getHeight(), img->getColorMode(), hBase->getHexes(), hBase->getNHex(), hBase->getHexMask(), hBase->getHexMaskPlus());
}
unsigned char DrawHexImg::Init(HexBase* hbase, s_hexPlate* plate) {
	unsigned char err_code = Init(hbase);
	m_nodes = plate->m_fhex;
	m_nHex = plate->m_nHex;
	m_defOCol.r = 0xFF;
	m_defOCol.g = 0xFF;
	m_defOCol.b = 0xFF;
	return ECODE_OK;
}
void DrawHexImg::Release()
{
	m_hex = NULL;
	m_nHex = 0;
	m_lowerHex = NULL;
	m_nLowerHex = 0; 
	m_hexMask = NULL;
	m_lowerHexMask = NULL;
	if (m_hexedImg != NULL) {
		m_hexedImg->release();
		delete m_hexedImg;
		m_hexedImg = NULL;
	}
}
unsigned char DrawHexImg::Run()
{
	return renderHexOuput();//renderHexImg();//genHexImgDebug();
}
unsigned char DrawHexImg::renderHexImg()
{
	for (int i = 0; i < m_nHex; i++) {
		s_hex curHex = m_hex[i];
		s_rgb hexCol = imgMath::convToRGB(curHex.rgb[0], curHex.rgb[1], curHex.rgb[2]);
		m_hexedImg->PrintMaskedImg(curHex.i, curHex.j, *m_hexMaskPlus, hexCol);
	}
	return ECODE_OK;
}
unsigned char DrawHexImg::renderLineImg(LineFinder* lineFinder) {
	return genLineImg(lineFinder);
}
unsigned char DrawHexImg::renderEyeImg(s_hexEye& eye) {
	return genEyeImgDebug(eye);
}
unsigned char DrawHexImg::genHexImgDebug()
{
	unsigned char err_code = ECODE_OK;
	/*gen image of hex mask*/
	s_rgb hexCol = { 0xff, 0xde, 0xee };
	char rc[3] = { (char)0x00, (char)0xcc, (char)0xff };

	/*draw hexes under selected hex*/
	s_hex chosenHex = m_hex[100];
	if (chosenHex.centerLink >= 0) {
		s_hex centerHex = m_lowerHex[chosenHex.centerLink];
		m_hexedImg->PrintMaskedImg(centerHex.i, centerHex.j, *m_lowerHexMask, hexCol);
		for (int i = 0; i < 6; i++) {
			s_hex lowHex = m_lowerHex[chosenHex.downLinks[i]];
			int mi = i % 3;
			hexCol.r = rc[mi];
			m_hexedImg->PrintMaskedImg(lowHex.i, lowHex.j, *m_lowerHexMask, hexCol);
		}
	}
	/*                             */
	return ECODE_OK;
}
unsigned char DrawHexImg::renderHexOuput() {
	for (int i = 0; i < m_nHex; i++) {
		float r = m_nodes[i].o * 0xff;// (float)m_defOCol.r;
		float g = m_nodes[i].o * 0xff;// (float)m_defOCol.g;
		float b = m_nodes[i].o * 0xff;// (float)m_defOCol.b;
		s_rgb hexCol = imgMath::convToRGB(r, g, b);
		/*debug*/
		if (m_nodes[i].o < 0.0)
			hexCol = imgMath::convToRGB(0, 0, 0);
		m_hexedImg->PrintMaskedImg(m_nodes[i].shex->i, m_nodes[i].shex->j, *m_hexMaskPlus, hexCol);
	}
	return ECODE_OK;
}
unsigned char DrawHexImg::genLineImg(LineFinder* lineFinder) {
	m_hexedImg->clearToChar(0x00);
	int nlines = lineFinder->getNLines();
	if (nlines <= 0)
		return ECODE_ABORT;
	s_line* lines = lineFinder->getLines();
	for (int i_line = 0; i_line < nlines; i_line++) {
		for (int i = 0; i < lines[i_line].n; i++) {
			s_linePoint& pt = lines[i_line].pts[i];
			s_rgb hexCol = genLineCol(pt.lunai);
			int hex_i = pt.hexi;
			m_hexedImg->PrintMaskedImg(m_hex[hex_i].i, m_hex[hex_i].j, *m_hexMaskPlus, hexCol);
		}
	}
	return ECODE_OK;
}
s_rgb DrawHexImg::genLineCol(int lunai) {
	s_rgb rgb = { 0xff, 0xff, 0xff };
	float mul1 = 0x33;
	float mul2 = 0x44;
	if (lunai < 3) {
		float r = mul1 * (float)lunai;
		float g = mul2 * (float)lunai;
		rgb = imgMath::convToRGB(r, g, 0xff);
	}
	else {
		float li = (float)lunai - 3;
		float g = mul2 * li;
		float b = mul1 * li;
		rgb = imgMath::convToRGB(0xff, g, b);
	}
	return rgb;
}
unsigned char DrawHexImg::genEyeImgDebug(s_hexEye& eye) {
	s_hexPlate& botPlate = eye.lev[eye.n - 1];
	s_hexPlate& topPlate = eye.lev[0];
	s_hexPlate& midPlate = eye.lev[1];
	s_2pt offset = { 200, 200 };
	//drawHexPlate(botPlate, m_hexMask, offset);
	s_rgb col = { 0x00, 0x22, 0x00 };
	s_fNode* topNode = &(topPlate.m_fhex[0]);
	drawLowerNodes(topNode, m_hexMask, offset, col);
	col.r = 0x22;
	col.g = 0x00;
	col.b = 0xaa;
	s_fNode* midNode = &(midPlate.m_fhex[6]);
	drawLowerNodes(midNode, m_hexMask, offset, col);
	//drawWebHexPlate(botPlate, offset, 5);
	return ECODE_OK;
}
unsigned char DrawHexImg::drawHexPlate(s_hexPlate& plate, Img* hexMask, s_2pt& offset) {
	s_rgb col = { 0x00, 0x00, 0x22 };
	//float width = (float)m_hexedImg->getWidth();
	unsigned char addc = 0x22;

	for (long i = 0; i < plate.m_nHex; i++) {
		long Ii = (long)floorf(plate.m_fhex[i].x + offset.x0);
		long Ij = (long)floorf(plate.m_fhex[i].y + offset.x1);
		colRotate(col, addc);
		m_hexedImg->PrintMaskedImg(Ii, Ij, *hexMask, col);
	}
	return ECODE_OK;
}
unsigned char DrawHexImg::drawWebHexPlate(s_hexPlate& plate, s_2pt& offset, int web_i) {
	s_rgb col = { 0xdd, 0xdd, 0xdd };
	unsigned char addc = 0x22;
	for (long i = 0; i < plate.m_nHex; i++) {
		s_2pt_i hcent;
		hcent.x0 = (long)floorf(plate.m_fhex[i].x + offset.x0);
		hcent.x1 = (long)floorf(plate.m_fhex[i].y + offset.x1);
		s_2pt_i nextCent;
		s_fNode* nextNode = (s_fNode*)plate.m_fhex[i].web[web_i];
		if (nextNode != NULL) {
			nextCent.x0 = (long)floorf(nextNode->x + offset.x0);
			nextCent.x1 = (long)floorf(nextNode->y + offset.x1);
			colRotate(col, addc, 0xdd);
			m_hexedImg->DrawLine(hcent, nextCent, col);
		}
	}
	return ECODE_OK;
}
unsigned char DrawHexImg::drawLowerNodes(s_fNode* hiNode, Img* hexMask, s_2pt& offset, s_rgb& col) {
	unsigned char addc = 0x22;
	for (int i = 0; i < 7; i++) {
		s_fNode* nd = (s_fNode*)hiNode->nodes[i];
		if (nd != NULL) {
			s_2pt_i hcent;
			hcent.x0 = (long)floorf(nd->x + offset.x0);
			hcent.x1 = (long)floorf(nd->y + offset.x1);
			colRotate(col, addc);
			m_hexedImg->PrintMaskedImg(hcent.x0, hcent.x1, *hexMask, col);
		}
	}
	return ECODE_OK;
}
void DrawHexImg::colRotate(s_rgb& col, unsigned char addc, unsigned char basecol) {
	if (col.b < 0xff) {
		col.b += addc;
		if (col.b > 0xff)
			col.b = 0xff;
	}
	else if (col.g < 0xff) {
		col.g += addc;
		if (col.g > 0xff)
			col.g = 0xff;
	}
	else if (col.r < 0xff) {
		col.r += addc;
		if (col.r > 0xff)
			col.r = 0xff;
	}
	else {
		col.r = basecol;
		col.g = basecol;
		col.b = addc;
	}
}