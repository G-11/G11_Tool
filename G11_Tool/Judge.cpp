#include "Judge.h"
//=============================================================================
//����:
//	pos0St			�Е��̐����̎n�_
//	pos0Ed			�Е��̐����̏I�_
//	pos1St			�����Е��̐����̎n�_
//	pos1Ed			�����Е��̐����̏I�_
//	OutPosCross		��_�̍��W
//	OutRate			�n�_�����_�܂ł̒����̐�����̔䗦
//	OutVecReflect	���˃x�N�g��
//=============================================================================
bool ColliLine2LineXZ(D3DXVECTOR3 pos0St,D3DXVECTOR3 pos0Ed,D3DXVECTOR3 pos1St,D3DXVECTOR3 pos1Ed,D3DXVECTOR3* OutPosCross,float* OutRate,D3DXVECTOR3* OutVecReflect)
{
	D3DXVECTOR3 vec0,vec1,vec2;

	vec0 = pos0Ed-pos0St;
	vec1 = pos1Ed-pos1St;
	vec2 = pos0St-pos1St;

	//vec0-vec1�̐����̊O��(vec0��vec1���E���Ȃ畉)
	float CrossProduct01 = vec0.z*vec1.x-vec0.x*vec1.z;

	if(CrossProduct01==0.0f)
	{//���s�Ȃ�I��
		return false;
	}

	//vec1-vec2�̐����̊O��(vec1��vec2���E���Ȃ畉)
	float CrossProduct12 = vec1.z*vec2.x-vec1.x*vec2.z;
	float Rate12 = CrossProduct12/CrossProduct01;
	if(Rate12 >= 0.0f && Rate12 <= 1.0f)
	{
		//vec0-vec1�̐����̊O�ςɑ΂���vec2-vec0�̐����̊O�ς̓�������Z�o
		//(vec1�̐�����vec0�̉E���̎��A1.0���傫��)
		//(vec1�̐�����vec0�̍����̎��A0.0��菬����)
		if(OutPosCross != nullptr)
		{//������������̌�_
			*OutPosCross = pos0St+vec0*Rate12;
			//OutPosCross = pos1Sr+vec1*Rate02
		}
		if(OutRate != nullptr)
		{//������̌�_�܂ł̓�����
			*OutRate =Rate12;
		}
		if(OutVecReflect != nullptr)
		{
			D3DXVECTOR3 vecMove,vecLine;
			vecMove = pos1Ed-pos1St;
			vecLine = pos0Ed-pos0St;
			
			//��������������m�̓��ς��Z�o
			float DotProduct = vecLine.x*vecMove.x+vecLine.z*vecMove.z;

			//������������̂Ȃ��p
			float ValueMove = sqrt(vecMove.x*vecMove.x+vecMove.z*vecMove.z);
			float ValueLine = sqrt(vecLine.x*vecLine.x+vecLine.z*vecLine.z);

			float Angle = -acosf(DotProduct/(ValueMove*ValueLine));
			//���˃x�N�g��
			D3DXVECTOR3 vecMoveNew = D3DXVECTOR3(cosf(Angle)*vecLine.x-sinf(Angle)*vecLine.z,0.0f,cosf(Angle)*vecLine.z+sinf(Angle)*vecLine.x);
			D3DXVec3Normalize(&vecMoveNew,&vecMoveNew);
			*OutVecReflect = vecMoveNew;
		}
		return true;
	}
	return false;
}

//===========================================================
//��`
//===========================================================
bool RectHit(float aPosX,float aPosZ,float aSizeX,float aSizeZ,float bPosX,float bPosZ,float bSizeX,float bSizeZ)
{
	return ((aPosX+(aSizeX/2))>(bPosX-(bSizeX/2))&&(bPosX+(bSizeX/2))>(aPosX-(aSizeX/2))&&(aPosZ+(aSizeZ/2))>(bPosZ-(bSizeZ/2))&&(bPosZ+(bSizeZ/2))>(aPosZ-(aSizeZ/2)));

}
bool RectHit(const D3DXVECTOR2 &aPos,const D3DXVECTOR2 &aSize,const D3DXVECTOR2 &bPos,const D3DXVECTOR2 &bSize)
{
	return ((aPos.x + (aSize.x / 2))>(bPos.x - (bSize.x / 2)) && (bPos.x + (bSize.x / 2))>(aPos.x - (aSize.x / 2)) && (aPos.y + (aSize.y / 2))>(bPos.y - (bSize.y / 2)) && (bPos.y + (bSize.y / 2))>(aPos.y - (aSize.y / 2)));
}
//===========================================================
//������
//===========================================================
bool CubeHit(D3DXVECTOR3 aPos,D3DXVECTOR3 aSize,D3DXVECTOR3 bPos,D3DXVECTOR3 bSize)
{
	return ((aPos.x+(aSize.x/2))>(bPos.x-(bSize.x/2))&&(bPos.x+(bSize.x/2))>(aPos.x-(aSize.x/2))&&
		(aPos.z+(aSize.z/2))>(bPos.z-(bSize.z/2))&&(bPos.z+(bSize.z/2))>(aPos.z-(aSize.z/2))&&
		(aPos.y+(aSize.y/2))>=bPos.y&&(bPos.y+(bSize.y/2))>=aPos.y);
}
//===========================================================
// �~��̓����蔻��
//===========================================================
bool CircleHit2D(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br)
{
	return ((bpos.x - apos.x)*(bpos.x - apos.x) + (bpos.y - apos.y)*(bpos.y - apos.y))<(ar*ar) + (br*br);

}

bool CircleHit3D(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br)
{
	return ((bpos.x-apos.x)*(bpos.x-apos.x)+(bpos.z-apos.z)*(bpos.z-apos.z))<(ar*ar)+(br*br);

}
//===========================================================
// �~��̓����蔻��
//===========================================================
bool SphereHit(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br)
{
	return ((bpos.x - apos.x)*(bpos.x - apos.x) + (bpos.y - apos.y)*(bpos.y - apos.y)+(bpos.z - apos.z)*(bpos.z - apos.z))<(ar*ar) + (br*br);

}
//===========================================================
//�~��
//===========================================================
bool CylinderHit(D3DXVECTOR3 aPos,float aR,float aH,D3DXVECTOR3 bPos,float bR,float bH)
{
	return ((bPos.x-aPos.x)*(bPos.x-aPos.x)+(bPos.z-aPos.z)*(bPos.z-aPos.z))<(aR*aR)+(bR*bR)&&
		(aPos.y<=bPos.y+bH && aPos.y+aH>=bPos.y);
}

//�~�Ɛ����̂����蔻��
bool CollisionLC(float line_x,float line_y,float line_size_x,float line_size_y,float cx,float cy,float cr)
{
	if ((line_size_x*(cx - line_x) + line_size_y*(cy - line_y)) <= 0)
	{
		// �n�_��ʂ餐����ɐ����Ȑ���u�����Ƃ���~�̒��S�������͈̔͊O�ɂ������Ƃ�
		// ������̎n�_����~�̒��S�܂ł̋����̂Q�棂Ƣ�~�̔��a�̂Q�棂Ƃ̔�r
		return (cr*cr >= (cx - line_x)*(cx - line_x) + (cy - line_y)*(cy - line_y));
	}
	else if (((-line_size_x)*(cx - (line_x + line_size_x)) + (-line_size_y)*(cy - (line_y + line_size_y))) <= 0)
	{
		// �I�_��ʂ餐����ɐ����Ȑ���u�����Ƃ���~�̒��S�������͈̔͊O�ɂ������Ƃ�
		// ������̏I�_����~�̒��S�܂ł̋����̂Q�棂Ƣ�~�̔��a�̂Q�棂Ƃ̔�r
		return (cr*cr >= (cx - (line_x + line_size_x))*(cx - (line_x + line_size_x)) + (cy - (line_y + line_size_y))*(cy - (line_y + line_size_y)));
	}
	else
	{
		// �����̎n�_�I�_�ɐ����������������Ƃ���~�̒��S�����͈͓̔��ɂ������Ƃ�

		float e = sqrt(line_size_x*line_size_x + line_size_y*line_size_y);	// �����x,y����������ΒP�x�N�g���ɂȂ�
		float c2 = (cx - line_x)*(cx - line_x) + (cy - line_y)*(cy - line_y);
		float b = (cx - line_x)*(line_size_x / e) + (cy - line_y)*(line_size_y / e);	// ���ςŎZ�o������וӂ̒���
		return (cr*cr >= c2 - b*b);
	}
}
//===========================================================
//�~�ƌ��݂̂�����̓����蔻��
//����:
//	D3DXVECTOR3 ���̎n�_
//	D3DXVECTOR3 ���̏I�_
//	float		���̑���
//	float		���̌X��
//	D3DXVECTOR3 �~�̒��S�ʒu
//	float		�~�̔��a
//===========================================================
bool CollisionLC(const D3DXVECTOR3& line_head,const D3DXVECTOR3& line_tail,float width,float angle,const D3DXVECTOR3& obj_pos,float r)
{
	D3DXVECTOR3 pos = line_head;
	D3DXVECTOR3 dis = line_head - line_tail;
	float length = t_sqrtF((dis.x*dis.x) + (dis.y*dis.y));
	float c2l = ((line_head.x - obj_pos.x)*(line_head.x - obj_pos.x)) + ((line_head.y - obj_pos.y)*(line_head.y - obj_pos.y));

	if (length*length > c2l)
	{
		D3DXVECTOR3 move;
		move.x = -sinf(angle)*width;
		move.y = -cosf(angle)*width;
		move.z = t_sqrtF((move.x*move.x) + (move.y*move.y));

		float len = 0;
		while (len < length)
		{
			if (CircleHit2D(pos,width / 2.0f,obj_pos,r))
			{
				return true;
			}

			pos.x += move.x;
			pos.y += move.y;
			len += move.z;
			if (len > length)
			{
				pos = line_tail;
			}
		}
	}
	return false;


}