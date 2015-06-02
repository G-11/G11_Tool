#ifndef _COLLISION_H_
#define _COLLISION_H_
#include "main.h"

class Collision
{
public:
	static bool Rect(float aPosX,float aPosY,float aSizeX,float aSizeY,float bPosX,float bPosY,float bSizeX,float bSizeY);
	static bool Rect(float aPosX,float aPosY,float aSize,float bPosX,float bPosY,float bSize);
	static bool Quad(const D3DXVECTOR3 pos1,const D3DXVECTOR3* quadPoint1,UINT num1,const D3DXVECTOR3 pos2,const D3DXVECTOR3* quadPoint2,UINT num2);
	static bool DotQuad(const D3DXVECTOR3& pos,const D3DXVECTOR3* quad,UINT num);
	static bool CircleQuad(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3* quad,UINT num);
	static bool CircleQuad(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3* quad,UINT num,const D3DXVECTOR3& speed,D3DXVECTOR3* reflectVec = nullptr);
	static bool Line(const D3DXVECTOR2& point1,const D3DXVECTOR2& vec1,const D3DXVECTOR2& point2,const D3DXVECTOR2& vec2,D3DXVECTOR3* reflectVec = nullptr);
	static bool Cube(const D3DXVECTOR3& aPos,const D3DXVECTOR3& aSize,const D3DXVECTOR3& bPos,const D3DXVECTOR3& bSize);
	static bool Circle(float ax,float ay,float ar,float bx,float by,float br);
	static bool Circle(const D3DXVECTOR3& aPos,float ar,const D3DXVECTOR3& bPos,float br,bool xz=false);
	static bool CircleDot(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3& point);
	static bool Sphere(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br);
	static bool Cylinder(const D3DXVECTOR3& aPos,float ar,float ah,const D3DXVECTOR3& bPos,float br,float bh);
	static bool LineCircle(float line_x,float line_y,float line_size_x,float line_size_y,float cx,float cy,float cr);
	static bool LineCircle(const D3DXVECTOR3& line_start,const D3DXVECTOR3& line_end,const D3DXVECTOR3& circle,float radius);
	static bool LineCircle2(const D3DXVECTOR3& line_head,const D3DXVECTOR3& line_tail,float width,float angle,const D3DXVECTOR3& obj_pos,float r);
	static bool LineLineXZ(const D3DXVECTOR3& pos0St,const D3DXVECTOR3& pos0Ed,const D3DXVECTOR3& pos1St,const D3DXVECTOR3& pos1Ed,D3DXVECTOR3* OutPosCross,float* OutRate,D3DXVECTOR3* OutVecReflect);

private:
	static D3DXVECTOR3 CreateReflectVec(const D3DXVECTOR3& point1Start,const D3DXVECTOR3& point1End,const D3DXVECTOR3& point2Start,const D3DXVECTOR3& point2End);
};

//=======================================================================================
//��`�̓����蔻��
//=======================================================================================
inline bool Collision::Rect(float aPosX,float aPosY,float aSizeX,float aSizeY,float bPosX,float bPosY,float bSizeX,float bSizeY)
{
	return ((aPosX + (aSizeX / 2.0f)) > (bPosX - (bSizeX / 2.0f)) && (bPosX + (bSizeX / 2.0f)) > (aPosX - (bSizeX / 2.0f))
		&& (aPosY + (aSizeY / 2.0f)) > (bPosY - (bSizeY / 2.0f)) && (bPosY + (bSizeY / 2.0f)) > (aPosY - (bSizeY / 2.0f)));
}
//=======================================================================================
//�����`�̓����蔻��
//=======================================================================================
inline bool Collision::Rect(float aPosX,float aPosY,float aSize,float bPosX,float bPosY,float bSize)
{
	return ((aPosX + (aSize / 2.0f)) > (bPosX - (bSize / 2.0f)) && (bPosX + (bSize / 2.0f)) > (aPosX - (bSize / 2.0f))
		&& (aPosY + (aSize / 2.0f)) > (bPosY - (bSize / 2.0f)) && (bPosY + (bSize / 2.0f)) > (aPosY - (bSize / 2.0f)));
}
//=======================================================================================
//�����̂̓����蔻��
//=======================================================================================
inline bool Collision::Cube(const D3DXVECTOR3& aPos,const D3DXVECTOR3& aSize,const D3DXVECTOR3& bPos,const D3DXVECTOR3& bSize)
{
	return ((aPos.x + (aSize.x / 2))>(bPos.x - (bSize.x / 2)) && (bPos.x + (bSize.x / 2))>(aPos.x - (aSize.x / 2)) &&
		(aPos.z + (aSize.z / 2))>(bPos.z - (bSize.z / 2)) && (bPos.z + (bSize.z / 2))>(aPos.z - (aSize.z / 2)) &&
		(aPos.y + (aSize.y / 2)) >= bPos.y && (bPos.y + (bSize.y / 2)) >= aPos.y);
}
//=======================================================================================
//�~�̓����蔻��
//=======================================================================================
inline bool Collision::Circle(float ax,float ay,float ar,float bx,float by,float br)
{
	float disx = (bx - ax) * (bx - ax);
	float disy = (by - ay) * (by - ay);
	float disr = (ar + br)*(ar + br);
	
	return ( (disx+disy) <= disr );
}
//=======================================================================================
//�~�̓����蔻��
// xz:true�Ȃ�XZ���W���g�p�Afalse�Ȃ�XY���W���g�p
//=======================================================================================
inline bool Collision::Circle(const D3DXVECTOR3& aPos,float ar,const D3DXVECTOR3& bPos,float br,bool xz)
{
	if (xz == true)
	{
		return Circle(aPos.x,aPos.z,ar,bPos.x,bPos.z,br);
	}
	else
	{
		return Circle(aPos.x,aPos.y,ar,bPos.x,bPos.y,br);
	}
}

inline bool Collision::CircleDot(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3& point)
{
	D3DXVECTOR2 vec = D3DXVECTOR2(circle.x,circle.y)-D3DXVECTOR2(point.x,point.y);
	float length = D3DXVec2LengthSq(&vec);

	return (length < radius*radius);
}
//=======================================================================================
//���̓����蔻��
//=======================================================================================
inline bool Collision::Sphere(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br)
{
	D3DXVECTOR3 dis;
	dis.x = (bpos.x - apos.x)*(bpos.x - apos.x);
	dis.y = (bpos.y - apos.y)*(bpos.y - apos.y);
	dis.z = (bpos.z - apos.z)*(bpos.z - apos.z);
	float disr = (br - ar)*(br - ar);

	return (dis.x+dis.y+dis.z)<=disr;
	
}
//=======================================================================================
//�~���̓����蔻��
//=======================================================================================
inline bool Collision::Cylinder(const D3DXVECTOR3& aPos,float ar,float ah,const D3DXVECTOR3& bPos,float br,float bh)
{
	return ((bPos.x - aPos.x)*(bPos.x - aPos.x) + (bPos.z - aPos.z)*(bPos.z - aPos.z))<(ar * ar) + (br * br) &&
		(aPos.y <= bPos.y + bh && aPos.y + ah >= bPos.y);
}
//=======================================================================================
//���Ɖ~�̓����蔻��
//=======================================================================================
inline bool Collision::LineCircle(float line_x,float line_y,float line_size_x,float line_size_y,float cx,float cy,float cr)
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

inline bool Collision::LineCircle(const D3DXVECTOR3& line_start,const D3DXVECTOR3& line_end,const D3DXVECTOR3& circle,float radius)
{
	D3DXVECTOR2 start(line_start.x,line_start.y);
	D3DXVECTOR2 end(line_end.x,line_end.y);
	D3DXVECTOR2 cir(circle.x,circle.y);
	D3DXVECTOR2 length(0,0);
	
	D3DXVECTOR2 vec1 = end-start;
	D3DXVECTOR2 vec2 = cir-start;

	float vec1Length = D3DXVec2LengthSq(&vec1);
	float vec2Length = D3DXVec2LengthSq(&vec2);
	float dot = D3DXVec2Dot(&vec1,&vec2);

	if (dot / vec1Length < 0 || dot / vec1Length > 1){ return false; }

	float phd2 = (dot*dot)/vec1Length;

	if ((vec2Length - phd2) < radius*radius)
	{
		return true;
	}
	
	return false;
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
inline bool Collision::LineCircle2(const D3DXVECTOR3& line_head,const D3DXVECTOR3& line_tail,float width,float angle,const D3DXVECTOR3& obj_pos,float r)
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
			if (Circle(pos,width / 2.0f,obj_pos,r))
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
inline bool Collision::LineLineXZ(const D3DXVECTOR3& pos0St,const D3DXVECTOR3& pos0Ed,const D3DXVECTOR3& pos1St,const D3DXVECTOR3& pos1Ed,D3DXVECTOR3* OutPosCross,float* OutRate,D3DXVECTOR3* OutVecReflect)
{
	D3DXVECTOR3 vec0,vec1,vec2;

	vec0 = pos0Ed - pos0St;
	vec1 = pos1Ed - pos1St;
	vec2 = pos0St - pos1St;

	//vec0-vec1�̐����̊O��(vec0��vec1���E���Ȃ畉)
	float CrossProduct01 = vec0.z*vec1.x - vec0.x*vec1.z;

	if (CrossProduct01 == 0.0f)
	{//���s�Ȃ�I��
		return false;
	}

	//vec1-vec2�̐����̊O��(vec1��vec2���E���Ȃ畉)
	float CrossProduct12 = vec1.z*vec2.x - vec1.x*vec2.z;
	float Rate12 = CrossProduct12 / CrossProduct01;
	if (Rate12 >= 0.0f && Rate12 <= 1.0f)
	{
		//vec0-vec1�̐����̊O�ςɑ΂���vec2-vec0�̐����̊O�ς̓�������Z�o
		//(vec1�̐�����vec0�̉E���̎��A1.0���傫��)
		//(vec1�̐�����vec0�̍����̎��A0.0��菬����)
		if (OutPosCross != nullptr)
		{//������������̌�_
			*OutPosCross = pos0St + vec0*Rate12;
			//OutPosCross = pos1Sr+vec1*Rate02
		}
		if (OutRate != nullptr)
		{//������̌�_�܂ł̓�����
			*OutRate = Rate12;
		}
		if (OutVecReflect != nullptr)
		{
			D3DXVECTOR3 vecMove,vecLine;
			vecMove = pos1Ed - pos1St;
			vecLine = pos0Ed - pos0St;

			//��������������m�̓��ς��Z�o
			float DotProduct = vecLine.x*vecMove.x + vecLine.z*vecMove.z;

			//������������̂Ȃ��p
			float ValueMove = sqrt(vecMove.x*vecMove.x + vecMove.z*vecMove.z);
			float ValueLine = sqrt(vecLine.x*vecLine.x + vecLine.z*vecLine.z);

			float Angle = -acosf(DotProduct / (ValueMove*ValueLine));
			//���˃x�N�g��
			D3DXVECTOR3 vecMoveNew = D3DXVECTOR3(cosf(Angle)*vecLine.x - sinf(Angle)*vecLine.z,0.0f,cosf(Angle)*vecLine.z + sinf(Angle)*vecLine.x);
			D3DXVec3Normalize(&vecMoveNew,&vecMoveNew);
			*OutVecReflect = vecMoveNew;
		}
		return true;
	}
	return false;
}
//=============================================================================
// ��]�����`���m�̓����蔻��
//�����F
// pos1 �c ��ڂ̋�`�̒��S���W
// quadPoint1 �c ��ڂ̋�`�̒��_�z��
// num1 �c ��ڂ̋�`�̒��_��
// pos2 �c 2�ڂ̋�`�̒��S���W
// quadPoint2 �c 2�ڂ̋�`�̒��_�z��
// num2 �c 2�ڂ̋�`�̒��_��
//=============================================================================
inline bool Collision::Quad(const D3DXVECTOR3 pos1,const D3DXVECTOR3* quadPoint1,UINT num1,const D3DXVECTOR3 pos2,const D3DXVECTOR3* quadPoint2,UINT num2)
{
	//���ꂼ��̋�`�̕ϓ������������Ă�����true
	for (UINT cnt = 0;cnt < num1;cnt++)
	{
		D3DXVECTOR2 point1(quadPoint1[cnt].x,quadPoint1[cnt].y);
		D3DXVECTOR2 vec1(quadPoint1[(cnt + 1) % num1].x - quadPoint1[cnt].x,quadPoint1[(cnt + 1) % num1].y - quadPoint1[cnt].y);
		for (UINT cnt2 = 0;cnt2 < num2;cnt2++)
		{
			D3DXVECTOR2 point2(quadPoint2[cnt2].x,quadPoint2[cnt2].y);
			D3DXVECTOR2 vec2(quadPoint2[(cnt2 + 1) % num2].x - quadPoint2[cnt2].x,quadPoint2[(cnt2 + 1) % num2].y - quadPoint2[cnt2].y);
			if (Line(point1,vec1,point2,vec2))
			{
				return true;
			}
		}
	}

	//�Е��̋�`�̒��S�������Е��̋�`�Ɋ܂܂�Ă����true
	if (DotQuad(pos2,quadPoint1,num1))
	{
		return true;
	}
	if (DotQuad(pos1,quadPoint2,num2))
	{
		return true;
	}

	return false;
}

inline bool Collision::Line(const D3DXVECTOR2& point1,const D3DXVECTOR2& vec1,const D3DXVECTOR2& point2,const D3DXVECTOR2& vec2,D3DXVECTOR3* reflectVec)
{
	D3DXVECTOR2 vec = point2 - point1;
	float Cross_v1_v2 = D3DXVec2CCW(&vec1,&vec2);

	if (Cross_v1_v2 == 0){ return false; }

	float Cross_vec_v1 = D3DXVec2CCW(&vec,&vec1);
	float Cross_vec_v2 = D3DXVec2CCW(&vec,&vec2);

	float t1 = Cross_vec_v2 / Cross_v1_v2;
	float t2 = Cross_vec_v1 / Cross_v1_v2;

	float eps = 0.00001f;

	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) 
	{
		// �������Ă��Ȃ�
		return false;
	}
	else
	{
		if (reflectVec != nullptr)
		{
			D3DXVECTOR3 point1Start(point1.x,point1.y,0);
			D3DXVECTOR3 point2Start(point2.x,point2.y,0);
			D3DXVECTOR3 point1End = point1Start + D3DXVECTOR3(vec1.x,vec1.y,0);
			D3DXVECTOR3 point2End = point2Start + D3DXVECTOR3(vec2.x,vec2.y,0);
			*reflectVec = CreateReflectVec(point1Start,point1End,point2Start,point2End);
		}
		return true;
	}
}

inline bool Collision::DotQuad(const D3DXVECTOR3& pos,const D3DXVECTOR3* quad,UINT num)
{
	D3DXVECTOR3 vecline;
	D3DXVECTOR3 vectarget;
	bool hit = true;
	for (UINT cnt = 0;cnt < num;cnt++)
	{
		vecline = quad[(cnt + 1) % num] - quad[cnt];
		vectarget = pos - quad[cnt];
		if ((vecline.y*vectarget.x - vecline.x*vectarget.y) > 0)
		{
			return false;
		}
	}

	return true;
}

inline bool Collision::CircleQuad(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3* quad,UINT num)
{
	for (UINT cnt = 0;cnt < num;cnt++)
	{
		if (CircleDot(circle,radius,quad[cnt]))
		{
			return true;
		}
	}
	for (UINT cnt = 0;cnt < num;cnt++)
	{
		if (LineCircle(quad[(cnt + 1) % num],quad[cnt],circle,radius))
		{
			return true;
		}
	}

	if (DotQuad(circle,quad,num))
	{
		return true;
	}

	return false;
}

inline bool Collision::CircleQuad(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3* quad,UINT num,const D3DXVECTOR3& speed,D3DXVECTOR3* reflectVec)
{
	D3DXVECTOR3 reflect(0,0,0);
	for (UINT cnt = 0;cnt < num;cnt++)
	{
		if (CircleDot(circle,radius,quad[cnt]))
		{
			if (reflectVec != nullptr){ *reflectVec = -speed; }
			return true;
		}
	}
	for (UINT cnt = 0;cnt < num;cnt++)
	{
		if (LineCircle(quad[(cnt + 1) % num],quad[cnt],circle,radius))
		{
			reflect = CreateReflectVec(quad[(cnt + 1) % num],quad[cnt],circle,circle+(speed*2.0f));
			*reflectVec = reflect;
			return true;
		}
	}

	if (DotQuad(circle,quad,num))
	{
		if (reflectVec != nullptr){ *reflectVec = reflect; }
		return true;
	}

	return false;
}

inline D3DXVECTOR3 Collision::CreateReflectVec(const D3DXVECTOR3& point1Start,const D3DXVECTOR3& point1End,const D3DXVECTOR3& point2Start,const D3DXVECTOR3& point2End)
{
	D3DXVECTOR3 vec0,vec1,vec2;

	D3DXVECTOR3 VecNormal, Vec1,Vec2;
	float Length1, Length2;

	D3DXVec3Normalize(&VecNormal, &(point1End - point1Start));
	Vec1 = point2Start - point1Start;
	Length1 = D3DXVec3Dot(&VecNormal, &Vec1);

	D3DXVECTOR3 Point11 = point1Start + (VecNormal*Length1);
	Point11 -= point2Start;

	D3DXVECTOR3 PointAStart = point2Start + (Point11*2.0f);

	Vec2 = point2End - point1Start;
	Length2 = D3DXVec3Dot(&VecNormal, &Vec2);

	D3DXVECTOR3 Point21 = point1Start + (VecNormal*Length2);
	Point21 -= point2End;

	D3DXVECTOR3 PointAEnd = point2End + (Point21*2.0f);

	D3DXVECTOR3 reflect = PointAEnd - PointAStart;
	D3DXVec3Normalize(&reflect, &reflect);

	return reflect;
}



#endif