#include <fstream>
#include "..\include\d3dUtil.h"
#include "..\include\skybox.h"

CSkyBox::CSkyBox()
{
}

CSkyBox::~CSkyBox()
{
	release();
}

void CSkyBox::draw()
{
	d3dObj->mD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	d3dObj->mD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3dObj->mD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	d3dObj->mMatrixStack->Push();
	D3DMATRIX *m = d3dObj->mMatrixStack->GetTop();
	m->_41 = m->_42 = m->_43 = 0;
	d3dObj->mD3DDevice->SetTransform(D3DTS_VIEW, m);

	d3dObj->mD3DDevice->SetStreamSource(0, mVB, 0, 5 * sizeof(float));
	d3dObj->mD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	for(int i = 0; i < 6; i += 1)
	{
		d3dObj->mD3DDevice->SetTexture(0, *mTex[i]);
		d3dObj->mD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
	d3dObj->mD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3dObj->mD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3dObj->mD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	d3dObj->mMatrixStack->Pop();
}

void CSkyBox::restore()
{
	d3dObj->mD3DDevice->CreateVertexBuffer(5 * sizeof(float) * 24, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT, &mVB, NULL);
	float *data = NULL;
	const float w = 1.f;
	mVB->Lock(0, 0, (void **) &data, 0);
	// front
	data[0] = -w;  data[1] = -w;  data[2] = -w;  data[3] = 0.f;  data[4] = 1.f;
	data[5] = w;   data[6] = -w;  data[7] = -w;  data[8] = 1.f;  data[9] = 1.f;
	data[10] = -w; data[11] = w;  data[12] = -w; data[13] = 0.f; data[14] = 0.f;
	data[15] = w;  data[16] = w;  data[17] = -w; data[18] = 1.f; data[19] = 0.f;
	//back
	data[20] = -w; data[21] = -w; data[22] = w; data[23] = 0.f; data[24] = 1.f;
	data[25] = w;  data[26] = -w; data[27] = w; data[28] = 1.f; data[29] = 1.f;
	data[30] = -w; data[31] = w;  data[32] = w; data[33] = 0.f; data[34] = 0.f;
	data[35] = w;  data[36] = w;  data[37] = w; data[38] = 1.f; data[39] = 0.f;
	// left
	data[40] = -w; data[41] = -w; data[42] = -w; data[43] = 0.f; data[44] = 1.f;
	data[45] = -w; data[46] = -w; data[47] = w;  data[48] = 1.f; data[49] = 1.f;
	data[50] = -w; data[51] = w;  data[52] = -w; data[53] = 0.f; data[54] = 0.f;
	data[55] = -w; data[56] = w;  data[57] = w;  data[58] = 1.f; data[59] = 0.f;
	// right
	data[60] = w; data[61] = -w; data[62] = -w; data[63] = 0.f; data[64] = 1.f;
	data[65] = w; data[66] = -w; data[67] = w;  data[68] = 1.f; data[69] = 1.f;
	data[70] = w; data[71] = w;  data[72] = -w; data[73] = 0.f; data[74] = 0.f;
	data[75] = w; data[76] = w;  data[77] = w;  data[78] = 1.f; data[79] = 0.f;
	// top
	data[80] = -w; data[81] = w; data[82] = -w; data[83] = 0.f; data[84] = 1.f;
	data[85] = -w; data[86] = w; data[87] = w;  data[88] = 1.f; data[89] = 1.f;
	data[90] = w;  data[91] = w; data[92] = -w; data[93] = 0.f; data[94] = 0.f;
	data[95] = w;  data[96] = w; data[97] = w;  data[98] = 1.f; data[99] = 0.f;
	// bottom
	data[100] = -w; data[101] = -w; data[102] = -w; data[103] = 0.f; data[104] = 1.f;
	data[105] = -w; data[106] = -w; data[107] = w;  data[108] = 1.f; data[109] = 1.f;
	data[110] = w;  data[111] = -w; data[112] = -w; data[113] = 0.f; data[114] = 0.f;
	data[115] = w;  data[116] = -w; data[117] = w;  data[118] = 1.f; data[119] = 0.f;
	mVB->Unlock();
}

void CSkyBox::release()
{
	if(mVB)
	{
		mVB->Release();
		mVB = NULL;
	}
}

void CSkyBox::setTexture(TSkyBoxTexDir aDir, CTexture *aTex)
{
	mTex[(uint8)aDir] = aTex;
}

void CSkyBox::setTextures(CTexture **aTex)
{
	for (uint8 i = 0; i < 6; i++)
		mTex[i] = aTex[i];
}
