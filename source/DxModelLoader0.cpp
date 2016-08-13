// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�w�t�@�C���ǂݍ��݃v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#define __DX_MAKE

#include "DxModelRead.h"

#ifndef DX_NON_MODEL

// �C���N���[�h ---------------------------------
#include "DxLog.h"
#include "DxChar.h"
#include "DxMemory.h"
#include "DxSystem.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------

#define TOKEN_NAME					(1)
#define TOKEN_STRING				(2)
#define TOKEN_INTEGER				(3)
#define TOKEN_GUID					(5)
#define TOKEN_INTEGER_LIST			(6)
#define TOKEN_FLOAT_LIST			(7)

#define TOKEN_OBRACE				(10)
#define TOKEN_CBRACE				(11)
#define TOKEN_OPAREN				(12)
#define TOKEN_CPAREN				(13)
#define TOKEN_OBRACKET				(14)
#define TOKEN_CBRACKET				(15)
#define TOKEN_OANGLE				(16)
#define TOKEN_CANGLE				(17)
#define TOKEN_DOT					(18)
#define TOKEN_COMMA					(19)
#define TOKEN_SEMICOLON				(20)
#define TOKEN_TEMPLATE				(31)
#define TOKEN_WORD					(40)
#define TOKEN_DWORD					(41)
#define TOKEN_FLOAT					(42)
#define TOKEN_DOUBLE				(43)
#define TOKEN_CHAR					(44)
#define TOKEN_UCHAR					(45)
#define TOKEN_SWORD					(46)
#define TOKEN_SDWORD				(47)
#define TOKEN_VOID					(48)
#define TOKEN_LPSTR					(49)
#define TOKEN_UNICODE				(50)
#define TOKEN_CSTRING				(51)
#define TOKEN_ARRAY					(52)

#define MAX_STACKNUM				(64)			// �K�w�\���̍ő吔
#define MAX_MODELNUM				(1000)			// ���f���f�[�^�̍ő吔
#define MAX_INDEXNUM				(32)			// �ő咸�_�C���f�b�N�X��

// �e���v���[�g�^�C�v��`
#define TEMP_STRING					(0)				// ������
#define TEMP_VECTOR					(1)				// �x�N�g���e���v���[�g
#define TEMP_COORDS2D				(2)				// �e�N�X�`�����W�Q����
#define TEMP_MATRIX4X4				(3)				// 4 x 4 �s��
#define TEMP_COLORRGBA				(4)				// �A���t�@�t���J���[
#define TEMP_COLORRGB				(5)				// �J���[
#define TEMP_MATERIAL				(6)				// �}�e���A��
#define TEMP_TEXTUREFILENAME		(7)				// �e�N�X�`���t�@�C���l�[��
#define TEMP_MESHFACE				(8)				// ���b�V���t�F�C�X
#define TEMP_MESHTEXTURECOORDS		(9)				// ���b�V���e�N�X�`�����W
#define TEMP_MESHNORMALS			(10)			// ���b�V���̖@��
#define TEMP_MESHMATERIALLIST		(11)			// ���b�V���}�e���A�����X�g
#define TEMP_INDEXEDCOLOR			(12)			// �C���f�b�N�X�J���[
#define TEMP_MESHVERTEXCOLORS		(13)			// ���b�V�����_�J���[
#define TEMP_MESH					(14)			// ���b�V��
#define TEMP_FRAMETRANSFORMMATRIX	(15)			// �t���[���̃��[�J���g�����X�t�H�[��
#define TEMP_FRAME					(16)			// �t���[��
#define TEMP_FLOATKEYS				(17)			// ���������_���̔z��y�єz����� float �̐�
#define TEMP_TIMEDFLOATKEYS			(18)			// �A�j���[�V�����Ŏg�p����镂�������_���̃Z�b�g�y�ѐ��̎���
#define TEMP_ANIMATIONKEY			(19)			// �A�j���[�V�����L�[�̃Z�b�g
#define TEMP_ANIMATIONOPTIONS		(20)			// �A�j���[�V�����I�v�V����
#define TEMP_ANIMATION				(21)			// �O�̃t���[�����Q�Ƃ���A�j���[�V�������i�[����
#define TEMP_ANIMATIONSET			(22)			// Animation �I�u�W�F�N�g���i�[����
#define TEMP_XSKINMESHHEADER		(23)			// �X�L�j���O���b�V���̏��
#define TEMP_SKINWEIGHTS			(24)			// �X�L�����b�V���̃E�G�C�g
#define TEMP_XFILE					(25)			// �w�t�@�C��
#define TEMP_DWORD					(26)			// �c�v�n�q�c
#define TEMP_WORD					(27)			// �v�n�q�c
#define TEMP_FLOAT					(28)			// �e�k�n�`�s
#define TEMP_NUM					(29)			// �Ή����Ă���e���v���[�g�̐�


// �e���v���[�g�̕�����e�[�u��
const char *TTable[29] = 
{
	"",
	"",
	"Coords2d",
	"Matrix4x4",
	"ColorRGBA",
	"ColorRGB",
	"Material",
	"TextureFilename",
	"MeshFace",
	"MeshTextureCoords",
	"MeshNormals",
	"MeshMaterialList",
	"IndexedColor",
	"MeshVertexColors",
	"Mesh",
	"FrameTransformMatrix",
	"Frame",
	"FloatKeys",
	"TimedFloatKeys",
	"AnimationKey",
	"AnimationOption",
	"Animation",
	"AnimationSet",
	"XSkinMeshHeader",
	"SkinWeights",
	"",
	"",
	"",
	""
} ;

// �I�u�W�F�N�g�f�[�^�A�h���X�Q�Ɨp�}�N��
#define DOFF( x ) ( ( BYTE * )(x) + sizeof( X_OBJECT ) )

// �I�u�W�F�N�g�̃A�h���X�Q�Ɨp�}�N��
#define OOFF( x ) ( ( X_OBJECT * )( ( BYTE * )(x) - sizeof( X_OBJECT ) ) )

// �f�t�H���g�̃X�L�b�v������
const char *DefSkipStr = " \r\n\t,;" ;

// �f�[�^�^�錾 ---------------------------------

// �������͗p������f�[�^�^
struct X_PSTRING
{
	char					*StrBuf ;							// ������f�[�^
	char					SkipStr[40] ;						// �X�L�b�v������
	int						SkipStrLen ;						// �X�L�b�v������̒���
	int						StrOffset ;							// ��������W
	int						StrSize ;							// ������̃T�C�Y
	bool					binf ;								// �o�C�i���f�[�^��( true:�o�C�i���f�[�^  false:�e�L�X�g�f�[�^ )
	bool					f64f ;								// ���������_�f�[�^�͂U�S�r�b�g�^��( true:�U�S�r�b�g  false:�R�Q�r�b�g )
} ;

// �X�g�����O�f�[�^�^
struct X_STRING
{
	char					*String ;
} ;

// �c�v�n�q�c
struct X_DWORD
{
	DWORD					d ;
} ;

// �v�n�q�c
struct X_WORD
{
	WORD					w ;
} ;

// �e�k�n�`�s
struct X_FLOAT
{
	float					f ;
} ;

// �x�N�g��
struct X_VECTOR
{
	union
	{
		struct 
		{
			float			x, y, z ;
		} dat ;
		VECTOR				v ;
	} ;
} ;

// �R�[�h�X�Q�c
struct X_COORDS2D
{
	float					u ;
	float					v ;
} ;

// �S���S�s��
struct X_MATRIX4X4
{
	union
	{
		float				matrix[4][4] ;
		MATRIX				ms ;
	} ;
} ;

// �A���t�@�t���J���[�f�[�^
struct X_COLORRGBA
{
	float					red ;
	float					green ;
	float					blue ;
	float					alpha ;
} ;

// �J���[�f�[�^
struct X_COLORRGB
{
	float					red ;
	float					green ;
	float					blue ;
} ;

// �}�e���A��
struct X_MATERIAL
{
	X_COLORRGBA				faceColor ;
	float					power ;
	X_COLORRGB				specularColor ;
	X_COLORRGB				emissiveColor ;

	struct X_TEXTUREFILENAME *TextureFileName ;					// ���̃}�e���A���Ɋ֘A�t�����Ă���e�N�X�`���t�@�C���l�[��

	MV1_MATERIAL_R			*MV1RMaterial ;						// ���̃}�e���A���Ɋ֘A�t����ꂽ MV1_MATERIAL_R �ւ̃|�C���^
	int						Index ;								// �C���f�b�N�X
} ;

// �e�N�X�`���t�@�C���l�[��
struct X_TEXTUREFILENAME
{
	X_STRING				filename ;							// �e�N�X�`���t�@�C����
	int						Index ;								// �C���f�b�N�X
} ;

// ���b�V����
struct X_MESHFACE
{
	DWORD					nFaceVertexIndices ;				// �ʒ�`�C���f�b�N�X�̐�
	DWORD					faceVertexIndicesFixedLength[ MAX_INDEXNUM ] ;	// �ʒ�`�C���f�b�N�X( �Œ蒷�o�b�t�@ )
	DWORD					*faceVertexIndices ;				// �ʒ�`�C���f�b�N�X( �Œ蒷�o�b�t�@�Ɏ��܂�Ȃ��Ƃ��Ɏg�p )

} ;

// ���b�V���e�N�X�`���[���W
struct X_MESHTEXTURECOORDS
{
	DWORD					nTextureCoords ;					// �e�N�X�`�����W�̐�
	X_COORDS2D				*textureCoords ;					// �e�N�X�`�����W
} ;

// ���b�V���@��
struct X_MESHNORMALS
{
	DWORD					nNormals ;							// �@���̐�
	X_VECTOR				*normals ;							// �@��
	DWORD					nFaceNormals ;						// �@���ʂ̐�
	X_MESHFACE				*faceNormals ;						// �@����
} ;

// ���b�V���}�e���A�����X�g
struct X_MESHMATERIALLIST
{
	DWORD					nMaterials ;						// �}�e���A���̎�ނ̐�
	DWORD					nFaceIndexes ;						// �}�e���A���C���f�b�N�X�̐�
	DWORD					*FaceIndexes ;						// �}�e���A���C���f�b�N�X

	X_MATERIAL				**MaterialList ;					// �֘A�t����Ă���}�e���A���̃��X�g
} ;

// �C���f�b�N�X�J���[
struct X_INDEXEDCOLOR
{
	DWORD					index ;								// �C���f�b�N�X
	X_COLORRGBA				indexColor ;						// �J���[
} ;

// ���b�V�����_�J���[
struct X_MESHVERTEXCOLORS
{
	DWORD					nVertexColors ;						// ���_�J���[�̐�
	X_INDEXEDCOLOR			*vertexColors ;						// ���_�J���[
} ;

// ���b�V��
struct X_MESH
{
	DWORD					nVertices ;							// ���W�̐�
	X_VECTOR				*vertices ;							// ���W
	DWORD					nFaces ;							// �ʂ̐�
	X_MESHFACE				*faces ;							// ��
	DWORD					nTriangles ;						// �O�p�`�|���S�����Z�̃|���S���̐�
	DWORD					nTriangleListTotal ;				// �O�p�`�|���S�����X�g�̑���

	X_MESHTEXTURECOORDS		*TextureCoord ;						// �e�N�X�`���[���W
	X_MESHNORMALS			*Normals ;							// �@���f�[�^
	X_MESHMATERIALLIST		*MaterialList ;						// �}�e���A�����X�g
	X_MESHVERTEXCOLORS		*VertexColor ;						// ���_�J���[

	int						SkinWeightsNum ;					// �X�L�����b�V���̃{�[���̐�
	struct X_SKINWEIGHTS	**SkinWeights ;						// �X�L�����b�V���̃f�[�^�ʒu
	struct X_FRAME			*Frame ;							// ���̃��b�V���̃t���[��

	MV1_FRAME_R				*MeshFrame ;
} ;

// �t���[��(�y�т��̂��ׂĂ̎q�I�u�W�F�N�g)�̃��[�J���g�����X�t�H�[��
struct X_FRAMETRANSFORMMATRIX
{
	X_MATRIX4X4				frameMatrix ;
} ;

// �t���[��
struct X_FRAME
{
	BYTE					Dummy ;

	X_FRAMETRANSFORMMATRIX	*TransformMatrix ;					// ���̃t���[���Ɋ֘A�t�����Ă���g�����X�t�H�[���}�g���b�N�X

	int                     MeshNum ;							// ���̃t���[���Ɋ֘A�t�����Ă��郁�b�V���̐�
	X_MESH                  **Mesh ;							// ���̃t���[���Ɋ֘A�t�����Ă��郁�b�V���̃f�[�^�ʒu

	MV1_FRAME_R				*MV1Frame ;							// ���̃t���[���Ɋ֘A�t����ꂽ MV1_FRAME_R �\���̂ւ̃|�C���^
	int						Index ;								// ���̃t���[���̃C���f�b�N�X
} ;

// ���������_���̔z��y�єz����� float �̐�
struct X_FLOATKEYS
{
	DWORD					nValues ;							// ���������_���̐�
	float					*values ;							// ���������_��
} ;

// �A�j���[�V�����Ŏg�p����镂�������_���̃Z�b�g�y�ѐ��̎���
struct X_TIMEDFLOATKEYS
{
	DWORD					time ;								// ����
	X_FLOATKEYS				tfkeys ;							// �A�j���[�V�����L�[
} ;

// �A�j���[�V�����L�[�̃Z�b�g
struct X_ANIMATIONKEY
{
	DWORD					keyType ;							// �A�j���[�V�����^�C�v( 0:��](�N�H�[�^�j�I��)  1:�g��  2:���s�ړ�  4:�s�� )
	DWORD					nKeys ;								// �L�[�̐�
	X_TIMEDFLOATKEYS		*keys ;								// �L�[
	int						TotalCount ;						// �L�[�̑�����
} ;

// �A�j���[�V�����I�v�V����
struct X_ANIMATIONOPTIONS
{
	DWORD					openclosed ;
	DWORD					positionquality ;
} ;

// �A�j���[�V����
struct X_ANIMATION
{
	BYTE					Dummy ;

	DWORD					AnimationKeysNum ;					// �A�j���[�V�����L�[�̐�
	X_ANIMATIONKEY			**AnimationKeys ;					// �A�j���[�V�����L�[�̃f�[�^�|�C���^
	X_FRAME					*Frame ;							// �Ή�����t���[���̃f�[�^�|�C���^
	X_ANIMATION				*ParentsAnimation ;					// �Ή�����t���[���̐e�t���[���ɑΉ����Ă���A�j���[�V�����f�[�^
	int						MaxCount ;							// �ő�A�j���[�V�����J�E���g�l
} ;

// �A�j���[�V�����Z�b�g
struct X_ANIMATIONSET
{
	BYTE					Dummy ;

	int						KeyDataSize ;						// �A�j���[�V�����L�[�f�[�^�T�C�Y
	int						MaxCount ;							// �ő�J�E���g�l
	int						AnimationNum ;						// �A�j���[�V������
	X_ANIMATION				**Animation ;						// �A�j���[�V�����f�[�^�ʒu
} ;

// �X�L�j���O���b�V���̏��
struct X_XSKINMESHHEADER
{
	WORD					nMaxSkinWeightsPerVertex ;
	WORD					nMaxSkinWeightsPerFace ;
	WORD					nBones ;
} ;

// �X�L�����b�V���̃E�G�C�g
struct X_SKINWEIGHTS
{
	X_STRING				transformNodeName ;					// �ϊ��Ɏg�p����{�[���̖��O
	DWORD					nWeights ;							// �E�G�C�g�l�̐�
	DWORD					*vertexIndices ;					// �E�G�C�g���e��������ΏۂƂȂ郁�b�V����̒��_�̃C���f�b�N�X
	float					*weights ;							// ���_�ɑ΂���E�G�C�g
	X_MATRIX4X4				matrixOffset ;						// �I�t�Z�b�g�s��

	X_FRAME					*Frame ;							// �ւ���Ă���t���[���̃f�[�^�ʒu
} ;

// �֘A�t������Ă���I�u�W�F�N�g�̏��
struct X_RELATION_OBJECT
{
	char					*ObjectName ;						// �֘A�t������Ă���I�u�W�F�N�g�̖��O
	int						ObjectNum ;							// �֘A�t������Ă���I�u�W�F�N�g�̐�
	int						ObjectMaxNum ;						// �֘A�t���ł���I�u�W�F�N�g�̍ő吔
	struct X_OBJECT			**Object ;							// �֘A�t������Ă���I�u�W�F�N�g�ւ̃|�C���^
} ;

// �I�u�W�F�N�g�f�[�^�^
struct X_OBJECT
{
	int						Type ;								// �e���v���[�g�^�C�v
	char					*Name ;								// �I�u�W�F�N�g��
	int						ObjectDataSize ;					// X_OBJECT �\���̂̃f�[�^�T�C�Y���܂߂��I�u�W�F�N�g�̃f�[�^�T�C�Y
	struct X_OBJECT			*ParentsData ;						// �e�I�u�W�F�N�g�̃f�[�^�|�C���^
	int						ChildNum ;							// �q�̐�
	struct X_OBJECT			*ChildFirstData ;					// �q�̍ŏ��̃f�[�^�̃|�C���^
	struct X_OBJECT			*ChildLastData ;					// �q�̍Ō�̃f�[�^�̃|�C���^
	struct X_OBJECT			*ChildNextData ;					// �����Ɠ����e�ɏ������Ă��鎟�̎q�̃f�[�^�̃|�C���^

	int						RelationObjectNum ;					// �֘A�t��������Ă���I�u�W�F�N�g�̐�
	int						RelationObjectMaxNum ;				// �֘A�t�����ł���I�u�W�F�N�g�̐�
	X_RELATION_OBJECT		*RelationObject ;					// �֘A�t��������Ă���I�u�W�F�N�g�̏��̔z��
//	struct X_OBJECT			**RelationObject ;					// �֘A�t��������Ă���I�u�W�F�N�g�ւ̃|�C���^
//	char					**RelationObjectName ;				// �֘A�t��������Ă���I�u�W�F�N�g�̖��O

	struct X_OBJECT			*NextData ;							// ���̃I�u�W�F�N�g�f�[�^�ւ̃|�C���^
} ;

// �X�^�b�N�����p�f�[�^�\���̌^
struct X_STACK
{
	int						StackNum ;							// �X�^�b�N�ɋl�܂�Ă���I�u�W�F�N�g�̐�
	X_OBJECT				*Stack[ MAX_STACKNUM ] ;			// �X�^�b�N�ɋl�܂�Ă���I�u�W�F�N�g�̏��
} ;

// �w�t�@�C���f�[�^�^
struct X_XFILE
{
	BYTE					Dummy ;
} ;

// �e�N�X�`���t�@�C���l�[�����X�g
struct X_TEXTUREFILENAME_LIST
{
	X_TEXTUREFILENAME		*FileName ;
	X_TEXTUREFILENAME_LIST	*Next ;
} ;

// �w�t�@�C�����f���f�[�^�^
struct X_MODEL
{
	X_OBJECT				*FirstObject ;						// ��[�̃I�u�W�F�N�g�f�[�^�ւ̃|�C���^
	X_OBJECT				*LastObject ;						// ���[�̃I�u�W�F�N�g�f�[�^�ւ̃|�C���^

	MEMINFO					*XModelMem ;						// X_MODEL �p�̃������A�h���X�ۑ��f�[�^�ւ̃|�C���^

	X_TEXTUREFILENAME_LIST	*FirstTextureFileName ;				// �e�N�X�`���t�@�C���l�[�����X�g

	int						FrameNum ;							// �t���[���̐�
	int						MaterialNum ;						// �}�e���A���̐�
	int						TextureNum ;						// �e�N�X�`���̐�
	int						MeshNum ;							// ���b�V���̐�
	int						SkinWeightNum ;						// �X�L�����b�V���̃E�G�C�g�Ɋւ�����̐�
	int						MeshMaterialNum ;					// ���b�V���Ŏg�p����Ă���}�e���A���̐�
	int						TriangleListNum ;					// �g���C�A���O�����X�g�̐�
	int						TriangleNum ;						// �g���C�A���O���̐�
	int						AnimKeyDataSize ;					// �A�j���[�V�����L�[�Ɏg�p����f�[�^�̃T�C�Y
	int						AnimKeySetNum ;						// �A�j���[�V�����L�[�Z�b�g�̐�
	int						AnimNum ;							// �A�j���[�V�����̐�
	int						AnimSetNum ;						// �A�j���[�V�����Z�b�g�̐�
	int						VertexDataSize ;					// ���_�f�[�^�̃T�C�Y
	int						IndexDataSize ;						// ���_�C���f�b�N�X�f�[�^�̃T�C�Y
	int						StringSize ;						// ������ۑ��p�ɕK�v�ȃ������̃T�C�Y
} ;

// �f�[�^�錾 -----------------------------------

// �֐��錾 -------------------------------------

// �N�H�[�^�j�I���֌W
extern	int		QTCreate( FLOAT4 *Out, float Angle, VECTOR *Vect ) ;										// �w�莲�E�w��p��]���邽�߂̃N�H�[�^�j�I�����쐬����
extern	int		MulQT( FLOAT4 *Out, FLOAT4 *In1, FLOAT4 *In2 ) ;											// �N�H�[�^�j�I�����m�̊|���Z
extern	int		MulQTVect( VECTOR *Out, FLOAT4 *In1, VECTOR *InV, FLOAT4 *In2 ) ;							// �C�ӂ̃N�H�[�^�j�I�����g�p���ăx�N�g������]������

// �x�N�g�����Z�֌W
extern	int		CrossVect( VECTOR *Out, VECTOR *In1, VECTOR *In2 ) ;										// �Q�x�N�g���̊O�ς����߂�
extern	float	DotVect( VECTOR *In1, VECTOR *In2 ) ;														// �Q�x�N�g���̓��ς����߂�
extern	int		MultiplyMatrix( MATRIX *Out, MATRIX *In1, MATRIX *In2 ) ;									// �s�񓯎m�̊|���Z

// �\����͗p�⏕�֐�
extern	int		StrCmp2( char *String, int StrLength, char *StrBuffer ) ;									// ��O�̕�����𒲂ׂ�֐�
extern	char	*KakkoSkip( char *StrPoint ) ;																// ���ɏo�Ă��钆����'{}'��ǂݔ�΂�
extern	char	*StrTorkn( char *SertchStr, const char *SkipString ) ;										// strtok�̃p�`����

// �������͌n�֐�
extern	int		SetPStr(		X_PSTRING *PStrBuf, char *String, int StrOffset, const char *SkipStr, int StrSize = -1, bool binf = false, bool f64f = false ) ;	// �������͗p�f�[�^���Z�b�g����
extern	int		PStrGet(		X_PSTRING *PStrBuf, char *StrBuf, size_t BufferBytes ) ;							// ���ɏo�Ă��镶������擾����
extern	int		PStrGet(		X_PSTRING *PStrBuf, char *StrBuf, size_t BufferBytes, const char *AddSkipStr ) ;	// ���ɏo�Ă��镶������擾����
extern	int		PStrGetNMP(		X_PSTRING *PStrBuf, char *StrBuf, size_t BufferBytes, const char *AddSkipStr = "" ) ;			// �����I�t�Z�b�g���ړ������Ɏ��̕�����𓾂�
extern	int		PStrKakkoSkip(	X_PSTRING *PString ) ;														// ���ɏo�Ă���w{...}�x��ǂݔ�΂�
extern	int		PStrMove(		X_PSTRING *PStrBuf, const char *CmpStr ) ;									// �w��̕����񒆂̂ǂꂩ�̕���������܂œǂݔ�΂�
extern	int		PStrMoveP(		X_PSTRING *PStrBuf, const char *CmpStr ) ;									// �w��̕����񒆂̂ǂꂩ�̕����̎��̕����܂œǂݔ�΂�
extern	int		PStrMoveNum(	X_PSTRING *PStrBuf ) ;														// �������o��܂œǂݔ�΂�
extern	int		PStrGetNameBP(	X_PSTRING *PStrBuf, char *StrBuf, size_t BufferBytes ) ;					// �o�C�i���f�[�^�� TOKEN_NAME �𓾂���g�[�N����i�߂�
extern	int		PStrGetNameB(	X_PSTRING *PStrBuf, char *StrBuf, size_t BufferBytes ) ;					// �o�C�i���f�[�^�� TOKEN_NAME �𓾂�
extern	int		PStrMoveOneB(	X_PSTRING *PStrBuf ) ;														// ���̃g�[�N���ʒu�ֈړ�����
extern	int		PStrMoveB(		X_PSTRING *PStrBuf, int Code ) ;											// �w��̃g�[�N��������܂Ńg�[�N����ǂݔ�΂�
extern	int		PStrMovePB(		X_PSTRING *PStrBuf, int Code ) ;											// �w��̃g�[�N���̎��̃g�[�N���܂œǂݔ�΂�
extern	int		PStrKakkoSkipB(	X_PSTRING *PString ) ;														// ���ɏo�Ă���w{...}�x��ǂݔ�΂�(�o�C�i���o�[�W����)

#define PSTRC(x)			( ( unsigned char )( (x)->StrBuf[ (x)->StrOffset ] ) )							// �|�C���g���Ă��镶���𓾂�
#define PSTRP(x,y)			( &( (x)->StrBuf[ (x)->StrOffset + (y) ] ) )									// �|�C���g���Ă���A�h���X�𓾂�
#define PSTRWORD(x,y)		( *( ( unsigned short * )PSTRP( (x), (y) ) ) )									// �|�C���g���Ă���A�h���X�̃��[�h�l�𓾂�
#define PSTRDWORD(x,y)		( *( ( unsigned int   * )PSTRP( (x), (y) ) ) )									// �|�C���g���Ă���A�h���X�̃_�u�����[�h�l�𓾂�
#define PSTRFLOAT(x,y)		( *( ( float          * )PSTRP( (x), (y) ) ) )									// �|�C���g���Ă���A�h���X�̃t���[�g�l�𓾂�
#define PSTRDOUBLE(x,y)		( ( float )( *( ( double * )PSTRP( (x), (y) ) ) ) )								// �|�C���g���Ă���A�h���X�̃_�u���l�𓾂�
#define PSTRMOV(x,y)		( (x)->StrOffset += (y) )														// �|�C���g���Ă���A�h���X���ړ�����

// �w�t�@�C����̓f�[�^�����֘A
static	X_OBJECT	*AddObject( int TempType, const char *Name, X_OBJECT *Parents, X_MODEL *Model ) ;		// �I�u�W�F�N�g�f�[�^��ǉ�����
extern	int			TerminateXModel( X_MODEL *Model ) ;														// �w�t�@�C���̉�͂œ����f�[�^���������
static	void		*GetRelationObject( int Type, X_OBJECT *Object ) ;										// �w��̃I�u�W�F�N�g�Ɋ֘A�̂���w��̃^�C�v�̃I�u�W�F�N�g�𓾂�
static	int			EnumRelationObject( int Type, int *NumBuf, void **AddresBuf, X_OBJECT *Object ) ;		// �w��̃I�u�W�F�N�g�Ɋ֘A�̂���w��̃^�C�v�̃I�u�W�F�N�g��񋓂���
static	void		*GetMatchNameObject( int Type, char *Name, X_MODEL *Model ) ;							// �w��̖��O�����I�u�W�F�N�g���擾����
//static	X_OBJECT	*GetPrevObject( X_MODEL *Model, X_OBJECT *Object ) ;									// �w��̃I�u�W�F�N�g�Ɠ����e�����ЂƂO�̃f�[�^���擾����

extern	int			AnalysXFile( char *StringBuffer, int StrSize, X_MODEL *Model ) ;						// �w�t�@�C���̃f�[�^����͂��A�ǂݍ���
extern	int			AnalysXData( X_MODEL *Model, MV1_MODEL_R *RModel ) ;									// �w�t�@�C���̃��f���f�[�^����͂��A���s�p�f�[�^�̏������s��

static	int			IncStackObject( X_STACK *Stack, X_OBJECT *Object ) ;									// �I�u�W�F�N�g���X�^�b�N�ɉ�����
static	int			DecStackObject( X_STACK *Stack ) ;														// �I�u�W�F�N�g�̃X�^�b�N������炷

// �e���v���[�g��͗p�֐�
static	int			TempString(					X_PSTRING *PStr, X_MODEL *Model, X_STRING				*String					) ;	// ������e���v���[�g�̉��
static	int			TempWord(					X_PSTRING *PStr, X_MODEL *Model, X_WORD					*word					) ;	// �_�u�����[�h�e���v���[�g�̉��
static	int			TempDword(					X_PSTRING *PStr, X_MODEL *Model, X_DWORD				*Dword					) ;	// �_�u�����[�h�e���v���[�g�̉��
static	int			TempFloat(					X_PSTRING *PStr, X_MODEL *Model, X_FLOAT				*Float					) ;	// �t���[�g�e���v���[�g�̉��
static	int			TempVector(					X_PSTRING *PStr, X_MODEL *Model, X_VECTOR				*Vector					) ;	// �x�N�g���e���v���[�g�̉��
static	int			TempCoords2d(				X_PSTRING *PStr, X_MODEL *Model, X_COORDS2D				*Coords2d				) ;	// �e�N�X�`�����W�Q�����e���v���[�g�̉��
static	int			TempMatrix4x4(				X_PSTRING *PStr, X_MODEL *Model, X_MATRIX4X4			*Matrix					) ;	// �S���S�s��e���v���[�g�̉��
static	int			TempColorRGBA(				X_PSTRING *PStr, X_MODEL *Model, X_COLORRGBA			*ColorRGBA				) ;	// �A���t�@���J���[�e���v���[�g�̉��
static	int			TempColorRGB(				X_PSTRING *PStr, X_MODEL *Model, X_COLORRGB				*ColorRGB				) ;	// �J���[�e���v���[�g�̉��
static	int			TempMaterial(				X_PSTRING *PStr, X_MODEL *Model, X_MATERIAL				*Material				) ;	// �}�e���A���e���v���[�g�̉��
static	int			TempTextureFilename(		X_PSTRING *PStr, X_MODEL *Model, X_TEXTUREFILENAME		*TexFileName			) ;	// �e�N�X�`���t�@�C���l�[���e���v���[�g�̉��
static	int			TempMeshFace(				X_PSTRING *PStr, X_MODEL *Model, X_MESHFACE				*MeshFace				) ;	// ���b�V���t�F�C�X�e���v���[�g�̉��
static	int			TempMeshTextureCoords(		X_PSTRING *PStr, X_MODEL *Model, X_MESHTEXTURECOORDS	*MeshTextureCoords		) ;	// ���b�V���e�N�X�`���[�R�[�h�X�e���v���[�g�̉��
static	int			TempMeshNormals(			X_PSTRING *PStr, X_MODEL *Model, X_MESHNORMALS			*MeshNormals			) ;	// ���b�V���m�[�}���X�e���v���[�g�̉��
static	int			TempMeshMaterialList(		X_PSTRING *PStr, X_MODEL *Model, X_MESHMATERIALLIST		*MeshMaterialList		) ;	// ���b�V���}�e���A�����X�g�e���v���[�g�̉��
static	int			TempIndexedColor(			X_PSTRING *PStr, X_MODEL *Model, X_INDEXEDCOLOR			*IndexedColor			) ;	// �C���f�b�N�X�J���[�̉��
static	int			TempMeshVertexColors(		X_PSTRING *PStr, X_MODEL *Model, X_MESHVERTEXCOLORS		*MeshVertexColors		) ;	// ���b�V�����_�J���[�̉��
static	int			TempMesh(					X_PSTRING *PStr, X_MODEL *Model, X_MESH					*Mesh					) ;	// ���b�V���e���v���[�g�̉��
static	int			TempFrameTransformMatrix(	X_PSTRING *PStr, X_MODEL *Model, X_FRAMETRANSFORMMATRIX	*FrameTransformMatrix	) ;	// �t���[��(�y�т��̂��ׂĂ̂��I�u�W�F�N�g)�̃��[�J���g�����X�t�H�[���e���v���[�g�̉��
static	int			TempFrame(					X_PSTRING *PStr, X_MODEL *Model, X_FRAME				*Frame					) ;	// �t���[���e���v���[�g�̉��
static	int			TempFloatKeys(				X_PSTRING *PStr, X_MODEL *Model, X_FLOATKEYS			*FloatKeys				) ;	// ���������_�̔z��y�єz����� float �̐��e���v���[�g�̉��
static	int			TempTimedFloatKeys(			X_PSTRING *PStr, X_MODEL *Model, X_TIMEDFLOATKEYS		*TimedFloatKeys			) ;	// �A�j���[�V�����Ŏg�p����镂�������_���̃Z�b�g�y�ю��Ԃ��`����e���v���[�g�̉��
static	int			TempAnimationKey(			X_PSTRING *PStr, X_MODEL *Model, X_ANIMATIONKEY			*AnimationKey			) ;	// �A�j���[�V�����L�[�̃Z�b�g�e���v���[�g�̉��
static	int			TempAnimationOptions(		X_PSTRING *PStr, X_MODEL *Model, X_ANIMATIONOPTIONS		*AnimationOptions		) ;	// �A�j���[�V�����I�v�V�����e���v���[�g�̉��
static	int			TempAnimation(				X_PSTRING *PStr, X_MODEL *Model, X_ANIMATION			*Animation				) ;	// �A�j���[�V�����e���v���[�g�̉��
static	int			TempAnimationSet(			X_PSTRING *PStr, X_MODEL *Model, X_ANIMATIONSET			*AnimationSet			) ;	// �A�j���[�V�����Z�b�g�e���v���[�g�̉��
static	int			TempXSkinMeshHeader(		X_PSTRING *PStr, X_MODEL *Model, X_XSKINMESHHEADER		*XSkinMeshHeader		) ;	// �X�L�j���O���b�V���̃w�b�_���e���v���[�g�̉��
static	int			TempSkinWeights(			X_PSTRING *PStr, X_MODEL *Model, X_SKINWEIGHTS			*SkinWeights			) ;	// �X�L�����b�V���̃E�G�C�g���e���v���[�g�̉��

// �v���O���� -----------------------------------

// �N�H�[�^�j�I���֌W

// ��]�p�N�H�[�^�j�I���̍쐬
int QTCreate( FLOAT4 *Out, float Angle, VECTOR *Vect )
{
	float r ;
	float SinA ;

	r = ( float )_SQRT( Vect->x * Vect->x + Vect->y * Vect->y + Vect->z * Vect->z ) ;  
	Vect->x /= r ;
	Vect->y /= r ;
	Vect->z /= r ;

	_SINCOS_PLATFORM( Angle / 20.0f, &SinA, &Out->w ) ;
//	SinA = ( float )sin( Angle / 20.0f ) ;
//	Out->w = ( float )cos( Angle / 20.0f ) ;
	Out->x = SinA * Vect->x ;
	Out->y = SinA * Vect->y ;
	Out->z = SinA * Vect->z ;

	return 0 ;
}

// �N�H�[�^�j�I���̊|���Z
int MulQT( FLOAT4 *Out, FLOAT4 *In1, FLOAT4 *In2 )
{
	VECTOR VectBuf ;

	Out->w = In1->w * In2->w - DotVect( ( VECTOR * )&In1->x, ( VECTOR * )&In2->x ) ; 
	CrossVect( &VectBuf, ( VECTOR * )&In1->x, ( VECTOR * )&In2->x ) ;
	Out->x = In1->w * In2->x + In2->w * In1->x + VectBuf.x ;
	Out->y = In1->w * In2->y + In2->w * In1->y + VectBuf.y ;
	Out->z = In1->w * In2->z + In2->w * In1->z + VectBuf.z ;

	return 0 ;
}

// �x�N�g���̉�]
int MulQTVect( VECTOR *Out, FLOAT4 *In1, VECTOR *InV, FLOAT4 *In2 )
{
	FLOAT4 Buf, OutBuf1, OutBuf2 ;

	*( ( VECTOR * )&Buf ) = *InV ;
	Buf.w = 0.0f  ;

	MulQT( &OutBuf1, In1, &Buf ) ;
	MulQT( &OutBuf2, &OutBuf1, In2 ) ;

	*Out = *( ( VECTOR * )&OutBuf2 ) ;

	return 0;
}










// �x�N�g�����Z�֌W

// �x�N�g���̊O��
int CrossVect( VECTOR *Out, VECTOR *In1, VECTOR *In2 )
{
	Out->x = ( In1->y * In2->z - In1->z * In2->y ) ;
	Out->y = -( In1->x * In2->z - In1->z * In2->x ) ;
	Out->z = ( In1->x * In2->y - In1->y * In2->x ) ;

	return 0 ;
}

// �x�N�g���̓���
float DotVect( VECTOR *In1, VECTOR *In2 )
{
	return ( In1->x * In2->x + In1->y * In2->y + In1->z * In2->z ) ;
}

// �s��̊|���Z
int MultiplyMatrix( MATRIX *Out, MATRIX *In1, MATRIX *In2 )
{
	MATRIX Op1 = *In1 , Op2 = *In2 ;

	Out->m[0][0] = Op1.m[0][0] * Op2.m[0][0] + Op1.m[0][1] * Op2.m[1][0] + Op1.m[0][2] * Op2.m[2][0] + Op1.m[0][3] * Op2.m[3][0] ;
	Out->m[0][1] = Op1.m[0][0] * Op2.m[0][1] + Op1.m[0][1] * Op2.m[1][1] + Op1.m[0][2] * Op2.m[2][1] + Op1.m[0][3] * Op2.m[3][1] ;
	Out->m[0][2] = Op1.m[0][0] * Op2.m[0][2] + Op1.m[0][1] * Op2.m[1][2] + Op1.m[0][2] * Op2.m[2][2] + Op1.m[0][3] * Op2.m[3][2] ;
	Out->m[0][3] = Op1.m[0][0] * Op2.m[0][3] + Op1.m[0][1] * Op2.m[1][3] + Op1.m[0][2] * Op2.m[2][3] + Op1.m[0][3] * Op2.m[3][3] ;

	Out->m[1][0] = Op1.m[1][0] * Op2.m[0][0] + Op1.m[1][1] * Op2.m[1][0] + Op1.m[1][2] * Op2.m[2][0] + Op1.m[1][3] * Op2.m[3][0] ;
	Out->m[1][1] = Op1.m[1][0] * Op2.m[0][1] + Op1.m[1][1] * Op2.m[1][1] + Op1.m[1][2] * Op2.m[2][1] + Op1.m[1][3] * Op2.m[3][1] ;
	Out->m[1][2] = Op1.m[1][0] * Op2.m[0][2] + Op1.m[1][1] * Op2.m[1][2] + Op1.m[1][2] * Op2.m[2][2] + Op1.m[1][3] * Op2.m[3][2] ;
	Out->m[1][3] = Op1.m[1][0] * Op2.m[0][3] + Op1.m[1][1] * Op2.m[1][3] + Op1.m[1][2] * Op2.m[2][3] + Op1.m[1][3] * Op2.m[3][3] ;

	Out->m[2][0] = Op1.m[2][0] * Op2.m[0][0] + Op1.m[2][1] * Op2.m[1][0] + Op1.m[2][2] * Op2.m[2][0] + Op1.m[2][3] * Op2.m[3][0] ;
	Out->m[2][1] = Op1.m[2][0] * Op2.m[0][1] + Op1.m[2][1] * Op2.m[1][1] + Op1.m[2][2] * Op2.m[2][1] + Op1.m[2][3] * Op2.m[3][1] ;
	Out->m[2][2] = Op1.m[2][0] * Op2.m[0][2] + Op1.m[2][1] * Op2.m[1][2] + Op1.m[2][2] * Op2.m[2][2] + Op1.m[2][3] * Op2.m[3][2] ;
	Out->m[2][3] = Op1.m[2][0] * Op2.m[0][3] + Op1.m[2][1] * Op2.m[1][3] + Op1.m[2][2] * Op2.m[2][3] + Op1.m[2][3] * Op2.m[3][3] ;

	Out->m[3][0] = Op1.m[3][0] * Op2.m[0][0] + Op1.m[3][1] * Op2.m[1][0] + Op1.m[3][2] * Op2.m[2][0] + Op1.m[3][3] * Op2.m[3][0] ;
	Out->m[3][1] = Op1.m[3][0] * Op2.m[0][1] + Op1.m[3][1] * Op2.m[1][1] + Op1.m[3][2] * Op2.m[2][1] + Op1.m[3][3] * Op2.m[3][1] ;
	Out->m[3][2] = Op1.m[3][0] * Op2.m[0][2] + Op1.m[3][1] * Op2.m[1][2] + Op1.m[3][2] * Op2.m[2][2] + Op1.m[3][3] * Op2.m[3][2] ;
	Out->m[3][3] = Op1.m[3][0] * Op2.m[0][3] + Op1.m[3][1] * Op2.m[1][3] + Op1.m[3][2] * Op2.m[2][3] + Op1.m[3][3] * Op2.m[3][3] ;

	// �I��
	return 0 ;
}










// �������r�֐�

// ��O�̕�����𒲂ׂ�֐�
int StrCmp2( char *String, int StrLength, char *StrBuffer )
{
	int i , j ;

	i = 0 ;
	while( i < StrLength && *( String - i ) == ' ' ) i ++ ;
	if( i == StrLength || *( String - i ) == '\n' ) return -1 ;

	while( i < StrLength && *( String - i ) != ' ' && *( String - i ) != '\n' ) i ++ ;
	if( i == StrLength || *( String - i ) == '\n' ) return -1 ;

	while( i < StrLength && *( String - i ) == ' ' ) i ++ ;
	if( i == StrLength || *( String - i ) == '\n' ) return -1 ;

	j = 0 ;
	while( j + i < StrLength && *( String - i - j ) != ' ' && *( String - i - j ) != '\n' ) j ++ ;
	_MEMCPY( StrBuffer, String - i - j + 1, ( size_t )j ) ;
	StrBuffer[j] = '\0' ;

	return 0 ;
}

// strtok�̃p�`����
char *StrTorkn( char *SertchStr, const char *SkipString )
{
	static char *SertchString = NULL ;
	int i , Length ;
	char *RetPoint ;

	if( SertchStr != NULL ) SertchString = SertchStr ;
	if( SertchString == NULL ) return NULL ;

	Length = ( int )_STRLEN( SkipString ) ;

	for( i = -1 ; i != Length ; SertchString ++ )
	{
		for( i = 0 ; i < Length && SkipString[i] != *SertchString && '\0' != *SertchString ; i ++ ){}
		if( '\0' == *SertchString )
		{
			SertchString = NULL ;
			return NULL ;
		}
		if( i == Length ) break ;
	}

	RetPoint = SertchString ;

	for( i = Length ; i == Length ; SertchString ++ )
	{
		for( i = 0 ; i < Length && SkipString[i] != *SertchString && '\0' != *SertchString ; i ++ ){}
		if( '\0' == *SertchString )
		{
			SertchString = NULL ;
			return RetPoint ;
		}
		if( i != Length ) break ;
	}

	*SertchString = '\0' ;
	SertchString ++ ;

	return RetPoint ;
}

// ���ɏo�Ă��钆����'{}'��ǂݔ�΂�
char *KakkoSkip( char *StrPoint )
{
	int KakkoNum ;

	// '{'������܂œǂݔ�΂�
	while( *StrPoint != '{' )
	{
		if( ( unsigned char )( *StrPoint ) == 0xff ) return NULL ;
		StrPoint ++ ;
	}
	StrPoint ++ ;
	KakkoNum = 1 ;

	while( KakkoNum != 0 )
	{
		switch( ( BYTE )*StrPoint )
		{
		case '{' : KakkoNum ++ ; break ;
		case '}' : KakkoNum -- ; break ;
		case 0xff : KakkoNum = 0 ; break ;
		}

		StrPoint ++ ;
	}

	// �I��
	return StrPoint ;
}

















// �������͌n�֐�

// �������͗p�f�[�^���Z�b�g����
extern int SetPStr( X_PSTRING *PStrBuf, char *String, int StrOffset, const char *SkipStr, int StrSize, bool binf, bool f64f )
{
	PStrBuf->binf = binf ;
	PStrBuf->f64f = f64f ;

	if( String )
	{
		PStrBuf->StrBuf = String ;
		PStrBuf->StrOffset = StrOffset ;
		if( StrSize == -1 && binf == false ) PStrBuf->StrSize = ( int )_STRLEN( String ) ;
		else PStrBuf->StrSize = StrSize ;
	}

	if( SkipStr )
	{
		_STRCPY_S( PStrBuf->SkipStr, sizeof( PStrBuf->SkipStr ), SkipStr ) ;
		PStrBuf->SkipStrLen = ( int )_STRLEN( SkipStr ) ;
	}

	// �I��
	return 0 ;
}

// ���ɏo�Ă��镶������擾����
extern int PStrGet( X_PSTRING *PStrBuf, char *StrBuf, size_t BufferBytes )
{
	int i ;
	size_t DestSize ;
	int Len ;

	Len = ( int )_STRLEN( PStrBuf->SkipStr ) ;

	// �X�L�b�v�����łȂ���������������
	while( PStrBuf->StrOffset != PStrBuf->StrSize )
	{
		if( ( PStrBuf->StrBuf[PStrBuf->StrOffset] == '/' && PStrBuf->StrBuf[PStrBuf->StrOffset+1] == '/' ) ||
			PStrBuf->StrBuf[PStrBuf->StrOffset] == '#' )
		{
			PStrBuf->StrOffset ++ ;
			while( PStrBuf->StrOffset != PStrBuf->StrSize )
			{
				if( CHECK_SHIFTJIS_2BYTE( PStrBuf->StrBuf[PStrBuf->StrOffset] ) )
				{
					PStrBuf->StrOffset += 2 ;
				}
				else
				{
					if( PStrBuf->StrBuf[PStrBuf->StrOffset] == '\n' ) break ;
					PStrBuf->StrOffset ++ ;
				}
			}
				
			if( PStrBuf->StrOffset == PStrBuf->StrSize ) return -1 ;
		}

		if( CHECK_SHIFTJIS_2BYTE( PStrBuf->StrBuf[PStrBuf->StrOffset] ) == FALSE )
		{
			for( i = 0 ; i < Len ; i ++ )
			{
				if( PStrBuf->StrBuf[PStrBuf->StrOffset] == PStrBuf->SkipStr[i] ) goto R1 ;
			}
		}
		break ;
R1 :
		if( CHECK_SHIFTJIS_2BYTE( PStrBuf->StrBuf[PStrBuf->StrOffset] ) )
		{
			PStrBuf->StrOffset += 2 ;
		}
		else
		{
			PStrBuf->StrOffset ++ ;
		}
	}
	if( PStrBuf->StrOffset == PStrBuf->StrSize ) return -1 ;

	// �X�L�b�v�����񂪗���܂ŕ�������擾����
	DestSize = 0 ;
	while( PStrBuf->StrOffset < PStrBuf->StrSize )
	{
		if( CHECK_SHIFTJIS_2BYTE( PStrBuf->StrBuf[PStrBuf->StrOffset] ) )
		{
			if( BufferBytes - DestSize >= 3 )
			{
				StrBuf[ DestSize     ] = PStrBuf->StrBuf[PStrBuf->StrOffset] ;
				StrBuf[ DestSize + 1 ] = PStrBuf->StrBuf[PStrBuf->StrOffset + 1] ;
				DestSize += 2 ;
			}
			PStrBuf->StrOffset += 2 ;
		}
		else
		{
			for( i = 0 ; i < Len ; i ++ )
			{
				if( PStrBuf->StrBuf[PStrBuf->StrOffset] == PStrBuf->SkipStr[i] ) goto R2 ;
			}

			if( BufferBytes - DestSize >= 2 )
			{
				StrBuf[ DestSize ] = PStrBuf->StrBuf[PStrBuf->StrOffset] ;
				DestSize ++ ;
			}
			PStrBuf->StrOffset ++ ;
		}
	}
R2 :
	StrBuf[ DestSize ] = '\0' ;

	// �I��
	return 0 ;
}

// ���ɏo�Ă��镶������擾����
extern int PStrGet( X_PSTRING *PStrBuf, char *StrBuf, size_t BufferBytes, const char *AddSkipStr )
{
	char SkipStr[64] ;
	int r ;

	_STRCPY_S( SkipStr, sizeof( SkipStr ), PStrBuf->SkipStr ) ;
	_STRCAT_S( PStrBuf->SkipStr, sizeof( PStrBuf->SkipStr ), AddSkipStr ) ;
	r = PStrGet( PStrBuf, StrBuf, BufferBytes ) ;
	_STRCPY_S( PStrBuf->SkipStr, sizeof( PStrBuf->SkipStr ), SkipStr ) ;

	// �I��
	return r ;
}

// �����I�t�Z�b�g���ړ������Ɏ��̕�����𓾂�
extern int PStrGetNMP( X_PSTRING *PStrBuf, char *StrBuf, size_t BufferBytes, const char *AddSkipStr )
{
	char SkipStr[42] ;
	int r ;
	int Offset ;

	Offset = PStrBuf->StrOffset ;
	_STRCPY_S( SkipStr, sizeof( SkipStr ), PStrBuf->SkipStr ) ;
	_STRCAT_S( PStrBuf->SkipStr, sizeof( PStrBuf->SkipStr ), AddSkipStr ) ;
	r = PStrGet( PStrBuf, StrBuf, BufferBytes ) ;
	_STRCPY_S( PStrBuf->SkipStr, sizeof( PStrBuf->SkipStr ), SkipStr ) ;
	PStrBuf->StrOffset = Offset ;

	// �I��
	return r ;
}

// ���ɏo�Ă���w{...}�x��ǂݔ�΂�
extern int PStrKakkoSkip( X_PSTRING *PString )
{
	int KakkoNum ;

	// '{'������܂œǂݔ�΂�
	while( PString->StrOffset != PString->StrSize )
	{
		if( CHECK_SHIFTJIS_2BYTE( PString->StrBuf[PString->StrOffset] ) )
		{
			PString->StrOffset += 2 ;
		}
		else
		{
			if( PString->StrBuf[PString->StrOffset] == '{' ) break ;
			PString->StrOffset ++ ;
		}
	}
	if( PString->StrOffset == PString->StrSize ) return -1 ;
	PString->StrOffset ++ ;
	KakkoNum = 1 ;

	while( PString->StrOffset != PString->StrSize && KakkoNum != 0 )
	{
		if( PString->StrBuf[PString->StrOffset] == '/' && PString->StrBuf[PString->StrOffset+1] == '/' )
		{
			PString->StrOffset += 2 ;
			while( PString->StrOffset != PString->StrSize )
			{
				if( CHECK_SHIFTJIS_2BYTE( PString->StrBuf[PString->StrOffset] ) )
				{
					PString->StrOffset += 2 ;
				}
				else
				{
					if( PString->StrBuf[PString->StrOffset] == '\n' ) break ;
					PString->StrOffset ++ ;
				}
			}
			if( PString->StrOffset == PString->StrSize ) return -1 ;
		}

		switch( PString->StrBuf[PString->StrOffset] )
		{
		case '{' :
			KakkoNum ++ ;
			PString->StrOffset ++ ;
			break ;

		case '}' :
			KakkoNum -- ;
			PString->StrOffset ++ ;
			break ;

		default :
			if( CHECK_SHIFTJIS_2BYTE( PString->StrBuf[PString->StrOffset] ) )
			{
				PString->StrOffset += 2 ;
			}
			else
			{
				PString->StrOffset ++ ;
			}
			break ;
		}
	}
	if( PString->StrOffset == PString->StrSize ) return -1 ;

	// �I��
	return 0 ;
}




// �w��̕����񒆂̂ǂꂩ�̕���������܂œǂݔ�΂�
extern int PStrMove( X_PSTRING *PStrBuf, const char *CmpStr )
{
	int i ;
	int Len ;

	Len = ( int )_STRLEN( CmpStr ) ;

	// �X�L�b�v�����łȂ���������������
	while( PStrBuf->StrOffset != PStrBuf->StrSize )
	{
		if( PStrBuf->StrBuf[PStrBuf->StrOffset] == '/' && PStrBuf->StrBuf[PStrBuf->StrOffset+1] == '/' )
		{
			PStrBuf->StrOffset += 2 ;
			while( PStrBuf->StrOffset != PStrBuf->StrSize )
			{
				if( CHECK_SHIFTJIS_2BYTE( PStrBuf->StrBuf[PStrBuf->StrOffset] ) )
				{
					PStrBuf->StrOffset += 2 ;
				}
				else
				{
					if( PStrBuf->StrBuf[PStrBuf->StrOffset] == '\n' ) break ;
					PStrBuf->StrOffset ++ ;
				}
			}
		}

		if( CHECK_SHIFTJIS_2BYTE( PStrBuf->StrBuf[PStrBuf->StrOffset] ) )
		{
			PStrBuf->StrOffset += 2 ;
		}
		else
		{
			for( i = 0 ; i < Len ; i ++ )
				if( PStrBuf->StrBuf[PStrBuf->StrOffset] == CmpStr[i] ) goto R1 ;
			PStrBuf->StrOffset ++ ;
		}
	}

R1 :
	// �I��
	return PStrBuf->StrOffset == PStrBuf->StrSize ? -1 : 0 ;
}

// �w��̕����񒆂̂ǂꂩ�̕����̎��̕����܂œǂݔ�΂�
extern int PStrMoveP( X_PSTRING *PStrBuf, const char *CmpStr )
{
	int r ;

	r = PStrMove( PStrBuf, CmpStr ) ;
	if( r != -1 ) PStrBuf->StrOffset ++ ;

	// �I��
	return r ;
}

// �������o��܂œǂݔ�΂�
extern int PStrMoveNum( X_PSTRING *PStrBuf )
{
	return PStrMove( PStrBuf, "-0123456789." ) ;
}

// ���̃g�[�N���ʒu�ֈړ�����
extern int PStrMoveOneB( X_PSTRING *PStrBuf )
{
	if( PStrBuf->StrSize < PStrBuf->StrOffset ) return -1 ;

	// �g�[�N���ɂ���Đi�߂�l���Z�o
	switch( PSTRWORD(PStrBuf,0) )
	{
	case TOKEN_NAME :         PSTRMOV(PStrBuf, 2 + 4 + PSTRDWORD(PStrBuf,2) ) ; break ;
	case TOKEN_STRING :       PSTRMOV(PStrBuf, 2 + 4 + PSTRDWORD(PStrBuf,2) + 2 ) ; break ;
	case TOKEN_INTEGER :      PSTRMOV(PStrBuf, 2 + 4 ) ; break ;
	case TOKEN_GUID :         PSTRMOV(PStrBuf, 2 + 4 + 2 + 2 + 8 ) ; break ;
	case TOKEN_INTEGER_LIST : PSTRMOV(PStrBuf, 2 + 4 + 4 * PSTRDWORD(PStrBuf,2) ) ; break ;
	case TOKEN_FLOAT_LIST :   PSTRMOV(PStrBuf, 2 + 4 + (PStrBuf->f64f ? 8 : 4) * PSTRDWORD(PStrBuf,2) ) ; break ;
	default :                 PSTRMOV(PStrBuf, 2) ; break ;
	}

	// �I��
	return 0 ;
}

// �o�C�i���f�[�^�� TOKEN_NAME �𓾂�
extern int PStrGetNameB( X_PSTRING *PStrBuf, char *StrBuf, size_t BufferBytes )
{
	if( PSTRWORD(PStrBuf,0) != TOKEN_NAME ) return -1 ;

	_STRNCPY_S( StrBuf, BufferBytes, PSTRP(PStrBuf,6), ( int )( PSTRDWORD(PStrBuf,2) + 1 ) ) ;
	if( PSTRDWORD(PStrBuf,2) * sizeof( char ) < BufferBytes )
	{
		StrBuf[PSTRDWORD(PStrBuf,2)] = '\0' ;
	}

	// �I��
	return 0 ;
}

// �o�C�i���f�[�^�� TOKEN_NAME �𓾂���g�[�N����i�߂�
extern int PStrGetNameBP( X_PSTRING *PStrBuf, char *StrBuf, size_t BufferBytes )
{
	if( PSTRWORD(PStrBuf,0) != TOKEN_NAME ) return -1 ;

	PStrGetNameB( PStrBuf, StrBuf, BufferBytes ) ;
	PStrMoveOneB( PStrBuf ) ;

	// �I��
	return 0 ;
}

// �w��̃g�[�N��������܂Ńg�[�N����ǂݔ�΂�
extern int PStrMoveB( X_PSTRING *PStrBuf, int Code )
{
	// �w��̃g�[�N��������܂œǂݔ�΂�
	while( PStrBuf->StrSize > PStrBuf->StrOffset && PSTRWORD(PStrBuf,0) != Code )
		PStrMoveOneB(PStrBuf) ;
	if( PStrBuf->StrSize <= PStrBuf->StrOffset ) return -1 ;

	// �I��
	return 0 ;
}

// �w��̃R�[�h�̎��̃g�[�N���܂œǂݔ�΂�
extern int PStrMovePB( X_PSTRING *PStrBuf, int Code )
{
	// �w��̃g�[�N��������܂œǂݔ�΂�
	PStrMoveB( PStrBuf, Code ) ;

	// �w��̃g�[�N�����炳��Ɉ��̃g�[�N���܂œǂ�
	PStrMoveOneB( PStrBuf ) ;

	return 0 ;
}

// ���ɏo�Ă���w{...}�x��ǂݔ�΂�(�o�C�i���o�[�W����)
extern int PStrKakkoSkipB( X_PSTRING *PString )
{
	int KakkoNum ;

	// '{'������܂œǂݔ�΂�
	PStrMovePB( PString, TOKEN_OBRACE ) ;
	if( PString->StrOffset >= PString->StrSize ) return -1 ;
	KakkoNum = 1 ;

	while( PString->StrOffset < PString->StrSize && KakkoNum != 0 )
	{
		switch( PSTRWORD(PString,0)  )
		{
		case TOKEN_OBRACE : KakkoNum ++ ; break ;
		case TOKEN_CBRACE : KakkoNum -- ; break ;
		}

		PStrMoveOneB( PString ) ;
	}
	if( PString->StrOffset >= PString->StrSize ) return -1 ;

	// �I��
	return 0 ;
}


















// �w�t�@�C���̉�͂œ����f�[�^���������
extern int TerminateXModel( X_MODEL *Model )
{
	X_MODEL *M = Model ;
	X_OBJECT *O, *OB ;

	// �f�[�^�m�F
	if( M == NULL ) return -1 ;

	// ���f���f�[�^���̃I�u�W�F�N�g���m�ۂ��Ă��郁���������ׂĉ��
	ClearMemArea( &Model->XModelMem ) ;

	// �I�u�W�F�N�g�p�Ɋm�ۂ��Ă��������������ׂĉ������
	O = M->FirstObject ;
	while( O != NULL )
	{
		OB = O->NextData ;
		DXFREE( O ) ;
		O = OB ;
	}
	M->FirstObject = NULL ;

	// �I��
	return 0 ;
}

// �I�u�W�F�N�g�f�[�^��ǉ�����
static X_OBJECT *AddObject( int TempType, const char *Name, X_OBJECT *Parents, X_MODEL *Model )
{
	X_OBJECT *O ;
	X_MODEL *M = Model ;
	int DataSize, NameLen ;

	if( M == NULL ) return NULL ;

	if( Name == NULL ) Name = "" ;

	// �m�ۂ��郁�����̃T�C�Y���f�[�^�̃^�C�v�ɂ���ĕύX����
	DataSize = sizeof( X_OBJECT ) ;
	switch( TempType )
	{
	case TEMP_STRING :					DataSize += sizeof( X_STRING ) ; break ;				// ������
	case TEMP_VECTOR :					DataSize += sizeof( X_VECTOR ) ; break ;				// �x�N�g���e���v���[�g
	case TEMP_COORDS2D :				DataSize += sizeof( X_COORDS2D ) ; break ;				// �e�N�X�`�����W�Q����
	case TEMP_MATRIX4X4 :				DataSize += sizeof( X_MATRIX4X4 ) ; break ;				// 4 x 4 �s��
	case TEMP_COLORRGBA :				DataSize += sizeof( X_COLORRGBA ) ; break ;				// �A���t�@�t���J���[
	case TEMP_COLORRGB :				DataSize += sizeof( X_COLORRGB ) ; break ;				// �J���[
	case TEMP_MATERIAL :				DataSize += sizeof( X_MATERIAL ) ; break ;				// �}�e���A��
	case TEMP_TEXTUREFILENAME :			DataSize += sizeof( X_TEXTUREFILENAME ) ; break ;		// �e�N�X�`���t�@�C���l�[��
	case TEMP_MESHFACE :				DataSize += sizeof( X_MESHFACE ) ; break ;				// ���b�V���t�F�C�X
	case TEMP_MESHTEXTURECOORDS :		DataSize += sizeof( X_MESHTEXTURECOORDS ) ; break ;		// ���b�V���e�N�X�`�����W
	case TEMP_MESHNORMALS :				DataSize += sizeof( X_MESHNORMALS ) ; break ;			// ���b�V���̖@��
	case TEMP_MESHMATERIALLIST :		DataSize += sizeof( X_MESHMATERIALLIST ) ; break ;		// ���b�V���}�e���A�����X�g
	case TEMP_INDEXEDCOLOR :			DataSize += sizeof( X_INDEXEDCOLOR ) ; break ;			// �C���f�b�N�X�J���[
	case TEMP_MESHVERTEXCOLORS :		DataSize += sizeof( X_MESHVERTEXCOLORS ) ; break ;		// ���b�V�����_�J���[
	case TEMP_MESH :					DataSize += sizeof( X_MESH ) ; break ;					// ���b�V��
	case TEMP_FRAMETRANSFORMMATRIX :	DataSize += sizeof( X_FRAMETRANSFORMMATRIX ) ; break ;	// �t���[���̃��[�J���g�����X�t�H�[��
	case TEMP_FRAME :					DataSize += sizeof( X_FRAME ) ; break ;					// �t���[��
	case TEMP_FLOATKEYS :				DataSize += sizeof( X_FLOATKEYS ) ; break ;				// ���������_���̔z��y�єz����� float �̐�
	case TEMP_TIMEDFLOATKEYS :			DataSize += sizeof( X_TIMEDFLOATKEYS ) ; break ;		// �A�j���[�V�����Ŏg�p����镂�������_���̃Z�b�g�y�ѐ��̎���
	case TEMP_ANIMATIONKEY :			DataSize += sizeof( X_ANIMATIONKEY ) ; break ;			// �A�j���[�V�����L�[�̃Z�b�g
	case TEMP_ANIMATIONOPTIONS :		DataSize += sizeof( X_ANIMATIONOPTIONS ) ; break ;		// �A�j���[�V�����I�v�V����
	case TEMP_ANIMATION :				DataSize += sizeof( X_ANIMATION ) ; break ;				// �O�̃t���[�����Q�Ƃ���A�j���[�V�������i�[����
	case TEMP_ANIMATIONSET :			DataSize += sizeof( X_ANIMATIONSET ) ; break ;			// Animation �I�u�W�F�N�g���i�[����
	case TEMP_XSKINMESHHEADER :			DataSize += sizeof( X_XSKINMESHHEADER ) ; break ;		// �X�L�j���O���b�V���̏��
	case TEMP_SKINWEIGHTS :				DataSize += sizeof( X_SKINWEIGHTS ) ; break ;			// �X�L�����b�V���̃E�G�C�g
	case TEMP_XFILE :					DataSize += sizeof( X_XFILE ) ; break ;					// �w�t�@�C��
	case TEMP_DWORD :					DataSize += sizeof( X_DWORD ) ; break ;					// �c�v�n�q�c
	case TEMP_WORD :					DataSize += sizeof( X_WORD ) ; break ;					// �v�n�q�c
	case TEMP_FLOAT :					DataSize += sizeof( X_FLOAT ) ; break ;					// �e�k�n�`�s
	}

	DWORD jbm ;

	// �������̈�̊m��
	NameLen = ( int )_STRLEN( Name ) + 1 ;
	jbm = ( DWORD )( DataSize + NameLen ) ;
	if( ( O = ( X_OBJECT * )DXALLOC( jbm ) ) == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Load XFile : �I�u�W�F�N�g�̃������̈�̊m�ۂɎ��s���܂���" @*/ ) ;
		return NULL ;
	}

	// ������̕ۑ��ɕK�v�ȃ������̑��T�C�Y�ɉ��Z
	Model->StringSize += ( NameLen + 3 ) / 4 * 4 ;

	// �f�[�^�̏�����
	_MEMSET( O, 0, ( size_t )( DataSize + ( int )_STRLEN( Name ) + 1 ) ) ;
	O->Type = TempType ;
	O->ObjectDataSize = DataSize ;
	O->Name = ( char * )( ( BYTE * )O + O->ObjectDataSize ) ;
	_STRCPY( O->Name, Name ) ;
	O->ParentsData = Parents ;
	O->ChildLastData = O->ChildFirstData = NULL ;
	O->ChildNextData = NULL ;
	O->NextData = NULL ;

	// �f�[�^�̊֘A�Â����{��
	if( Parents != NULL )
	{
		if( Parents->ChildFirstData == NULL )
		{
			Parents->ChildFirstData = Parents->ChildLastData = O ;
		}
		else
		{
			Parents->ChildLastData = ( Parents->ChildLastData->ChildNextData = O ) ;
		}
	}

	// �I�u�W�F�N�g�f�[�^��ǉ�����
	if( M->FirstObject == NULL )
	{
		M->FirstObject = M->LastObject = O ;
	}
	else
	{
		M->LastObject = ( M->LastObject->NextData = O ) ;
	}

	// �A�h���X��Ԃ�
	return O ;
}

// �w��̃I�u�W�F�N�g�Ɋ֘A�̂���w��̃^�C�v�̃I�u�W�F�N�g�𓾂�
static void *GetRelationObject( int Type, X_OBJECT *Object )
{
	int i ;
	int j ;
	X_OBJECT *SO ;

	// �w��^�C�v�̃I�u�W�F�N�g���q�ɂ��邩���ׂ�
	SO = Object->ChildFirstData ;
	while( SO != NULL )
	{
		if( Type == SO->Type ) break ;
		SO = SO->ChildNextData ;
	}

	// �����q�I�u�W�F�N�g�Ɏw��^�C�v�̃I�u�W�F�N�g�����Ȃ������ꍇ�͊֘A�t������Ă���I�u�W�F�N�g����T��
	if( SO == NULL )
	{
		for( i = 0 ; i < Object->RelationObjectNum ; i ++ )
		{
			for( j = 0 ; j < Object->RelationObject[ i ].ObjectNum ; j ++ )
			{
				if( Object->RelationObject[ i ].Object[ j ]->Type == Type )
				{
					SO = Object->RelationObject[ i ].Object[ j ] ;
					break ;
				}
			}
			if( j != Object->RelationObject[ i ].ObjectNum )
			{
				break ;
			}
		}
	}

	// �������I�u�W�F�N�g�̃A�h���X��Ԃ�
	return SO != NULL ? ( void * )DOFF( SO ) : NULL ;
}

// �w��̃I�u�W�F�N�g�Ɋ֘A�̂���w��̃^�C�v�̃I�u�W�F�N�g��񋓂���
static int EnumRelationObject( int Type, int *NumBuf, void **AddresBuf, X_OBJECT *Object )
{
	int i ;
	int j ;
	int Num ;
	X_OBJECT *SO ;

	// �w��^�C�v�̃I�u�W�F�N�g���q�ɂ��邩���ׂ�
	SO = Object->ChildFirstData ;
	Num = 0 ;
	while( SO != NULL )
	{
		if( Type == SO->Type )
		{
			Num ++ ;
			if( AddresBuf != NULL )
			{
				*AddresBuf = ( void * )DOFF( SO ) ;
				AddresBuf ++ ;
			}
		}

		SO = SO->ChildNextData ;
	}

	// �֘A�t������Ă���I�u�W�F�N�g������T��
	for( i = 0 ; i < Object->RelationObjectNum ; i ++ )
	{
		for( j = 0 ; j < Object->RelationObject[ i ].ObjectNum ; j ++ )
		{
			if( Object->RelationObject[ i ].Object[ j ]->Type == Type )
			{
				Num ++ ;
				if( AddresBuf != NULL )
				{
					*AddresBuf = ( void * )DOFF( Object->RelationObject[ i ].Object[ j ] ) ;
					AddresBuf ++ ;
				}
			}
		}
	}

	// ���o�b�t�@�Ɋi�[
	if( NumBuf != NULL ) *NumBuf = Num ;

	// �I��
	return 0 ;
}

// �w��̖��O�����I�u�W�F�N�g���擾����
static void *GetMatchNameObject( int Type, char *Name, X_MODEL *Model )
{
	X_MODEL *M ;
	X_OBJECT *O ;

	M = Model ;
	if( M == NULL ) return NULL ;

	O = M->FirstObject ;
	while( O != NULL )
	{
		if( O->Type == Type && _STRCMP( O->Name, Name ) == 0 ) break ;
		O = O->NextData ;
	}

	// �I��
	return O != NULL ? DOFF( O ) : NULL ;
}

/*
// �w��̃I�u�W�F�N�g�Ɠ����e�����ЂƂO�̃f�[�^���擾����
static	X_OBJECT	*GetPrevObject( X_MODEL *Model, X_OBJECT *Object )
{
	X_OBJECT *BO, *O ;

	BO = NULL ;
	O = Object->ParentsData == NULL ? Model->FirstObject : Object->ParentsData->ChildFirstData ;
	while( O != Object )
	{
		if( O->ParentsData == Object->ParentsData )
			BO = O ;
		O = O->NextData ;
	}

	return BO ;
}
*/


// �w�t�@�C���̃f�[�^����͂��A�ǂݍ���
extern int AnalysXFile( char *StringBuffer, int StrSize, X_MODEL *Model )
{
	X_PSTRING PS ;
	X_MODEL *M ;
	X_STACK Stack ;
	char StrB[256], Name[128] ;
	int Temp, Ret, Len, i ;
	X_OBJECT *O, *Ob ;
	bool binf, f64f ;

	// �e�L�X�g�w�t�@�C�����`�F�b�N
	if( _STRNCMP( StringBuffer   , "xof ", 4 ) != 0 ) goto ERR ;
//	if( _STRNCMP( StringBuffer+ 4, "0302", 4 ) != 0 ) goto ERR ;
	if( _STRNCMP( StringBuffer+ 8, "txt ", 4 ) != 0 && _STRNCMP( StringBuffer+ 8, "bin ", 4 ) != 0 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xfe\x5b\xdc\x5f\x57\x30\x66\x30\x44\x30\x8b\x30\x20\x00\x78\x00\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x62\x5f\x0f\x5f\x6f\x30\xc6\x30\xad\x30\xb9\x30\xc8\x30\x62\x5f\x0f\x5f\x68\x30\x5e\x97\x27\x57\x2e\x7e\xd0\x30\xa4\x30\xca\x30\xea\x30\x62\x5f\x0f\x5f\x6e\x30\x7f\x30\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�Ή����Ă��� x�t�@�C���`���̓e�L�X�g�`���Ɣ񈳏k�o�C�i���`���݂̂ł�\n" @*/ ) ;
		goto ERR ;
	}
//	if( _STRNCMP( StringBuffer+12, "0064", 4 ) != 0 ) goto ERR ;
	binf = _STRNCMP( StringBuffer+ 8, "txt ", 4 ) == 0 ? false : true ;
	f64f = _STRNCMP( StringBuffer+12, "0064", 4 ) == 0 ? true : false ;
	goto R1 ;

ERR :
//	return DXST_ERRORLOG_ADDW( L"�e�L�X�g�̂w�t�@�C���ł͂���܂���\n" ) ;
//	return DXST_ERRORLOG_ADDW( L"�w�t�@�C���ł͂���܂���\n" ) ;
	return -1 ;

R1 :
	// �������͗p�̃f�[�^���Z�b�g
	SetPStr( &PS, StringBuffer + 16, 0, DefSkipStr, StrSize - 16, binf, f64f ) ;  

	// ���f���f�[�^�̎擾
	M = Model ;
	if( M == NULL ) return -1 ;

	// �X�^�b�N��������������
	Stack.StackNum = 0 ;
	IncStackObject( &Stack, M->FirstObject ) ;
	O = Stack.Stack[Stack.StackNum-1] ;

	// �f�[�^�̉�͊J�n
	for(;;)
	{
		// �ŏ��̕�����𓾂�
		if( !binf )
		{
			if( PStrGetNMP( &PS, StrB, sizeof( StrB ) ) == -1 )
			{
				// �t�@�C���̏I�[�ɗ��Ă����烋�[�v���甲����
				if( Stack.StackNum != 1 )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x88\x4e\x1f\x67\x57\x30\x6a\x30\x44\x30\x20\x00\x45\x00\x4f\x00\x46\x00\x20\x00\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�\�����Ȃ� EOF �ł�\n" @*/ ) ;
					return -1 ;
				}
				break ;
			}
		}
		else
		{
			if( PS.StrOffset >= PS.StrSize )
			{
				// �t�@�C���̏I�[�ɗ��Ă����烋�[�v���甲����
				if( Stack.StackNum != 1 )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x88\x4e\x1f\x67\x57\x30\x6a\x30\x44\x30\x20\x00\x45\x00\x4f\x00\x46\x00\x20\x00\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�\�����Ȃ� EOF �ł�\n" @*/ ) ;
					return -1 ;
				}
				break ;
			}
		}

		// �����Ȃ�J�����ʁw{�x�̏ꍇ�͊ԐڎQ�ƂƂ݂Ȃ�
		if( ( !binf && StrB[0] == '{' ) || 
			( binf && PSTRWORD(&PS,0) == TOKEN_OBRACE ) )
		{
			// �I�u�W�F�N�g�����擾����
			if( binf )
			{
				PStrMovePB( &PS, TOKEN_OBRACE ) ;
				PStrGetNameBP( &PS, Name, sizeof( Name ) ) ;
				PStrMovePB( &PS, TOKEN_CBRACE ) ;
			}
			else
			{
				PStrMoveP( &PS, "{" ) ;
				PStrGetNMP( &PS, Name, sizeof( Name ) ) ;
				if( _STRCHR( Name, '}' ) != NULL )
				{
					PStrGetNMP( &PS, Name, sizeof( Name ), "}" ) ;
				}
				PStrMoveP( &PS, "}" ) ;
			}

			// �w��̃I�u�W�F�N�g�̖��O��ۑ�����
			if( O->RelationObjectMaxNum == O->RelationObjectNum )
			{
				void *OldRelationObject ;
				int NewMaxNum ;

				OldRelationObject     = O->RelationObject ;
				NewMaxNum             = O->RelationObjectMaxNum + 32 ;
				O->RelationObject     = ( X_RELATION_OBJECT * )ADDMEMAREA( sizeof( X_RELATION_OBJECT ) * NewMaxNum, &M->XModelMem ) ;
				if( O->RelationObject == NULL )
				{
					return DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x93\x95\xa5\x63\xc2\x53\x67\x71\x28\x75\xdd\x30\xa4\x30\xf3\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �ԐڎQ�Ɨp�|�C���^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				}
				if( O->RelationObjectMaxNum != 0 )
				{
					_MEMCPY( O->RelationObject, OldRelationObject, sizeof( X_RELATION_OBJECT ) * O->RelationObjectMaxNum ) ;
				}
				O->RelationObjectMaxNum = NewMaxNum ;
/*
				void *OldRelationObject, *OldRelationObjectName ;
				int NewMaxNum ;

				OldRelationObject     = O->RelationObject ;
				OldRelationObjectName = O->RelationObjectName ;
				NewMaxNum             = O->RelationObjectMaxNum + 32 ;
				O->RelationObject     = ( X_OBJECT ** )ADDMEMAREA( ( sizeof( X_OBJECT * ) + sizeof( char * ) ) * NewMaxNum, &M->XModelMem ) ;
				if( O->RelationObject == NULL )
					return DXST_ERRORLOGFMT_ADDW(( L"Load XFile : �ԐڎQ�Ɨp�|�C���^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" )) ;
				O->RelationObjectName = ( char ** )( O->RelationObject + NewMaxNum ) ;
				if( O->RelationObjectMaxNum != 0 )
				{
					_MEMCPY( O->RelationObject,     OldRelationObject,     sizeof( X_OBJECT * ) * O->RelationObjectMaxNum ) ;
					_MEMCPY( O->RelationObjectName, OldRelationObjectName, sizeof( char * )     * O->RelationObjectMaxNum ) ;
				}
				O->RelationObjectMaxNum = NewMaxNum ;
*/
			}

			Len = ( int )_STRLEN( Name ) ;

			O->RelationObject[ O->RelationObjectNum ].ObjectNum    = 0 ;
			O->RelationObject[ O->RelationObjectNum ].ObjectMaxNum = 0 ;
			O->RelationObject[ O->RelationObjectNum ].Object       = NULL ;
			O->RelationObject[ O->RelationObjectNum ].ObjectName   = ( char * )ADDMEMAREA( ( size_t )( Len + 1 ), &M->XModelMem ) ;
			if( O->RelationObject[ O->RelationObjectNum ].ObjectName == NULL )
				return DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x93\x95\xa5\x63\xc2\x53\x67\x71\x6e\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x0d\x54\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"Load XFile : �ԐڎQ�Ƃ̃I�u�W�F�N�g����ۑ����郁�����̈�̊m�ۂɎ��s���܂���" @*/ )) ;
			_STRCPY( O->RelationObject[ O->RelationObjectNum ].ObjectName, Name ) ;
/*
			O->RelationObjectName[ O->RelationObjectNum ] = ( char * )ADDMEMAREA( Len + 1, &M->XModelMem ) ;
			if( O->RelationObjectName[ O->RelationObjectNum ] == NULL )
				return DXST_ERRORLOGFMT_ADDW(( L"Load XFile : �ԐڎQ�Ƃ̃I�u�W�F�N�g����ۑ����郁�����̈�̊m�ۂɎ��s���܂���" )) ;
			_STRCPY( O->RelationObjectName[ O->RelationObjectNum ], Name ) ;
*/
			O->RelationObjectNum ++ ;
		}
		else
		{
			// �����ʂ̏ꍇ�̏���
			if( ( !binf && _STRCHR( StrB, '}' ) != NULL ) ||
				( binf && PSTRWORD(&PS,0) == TOKEN_CBRACE ) )
			{
				// �X�^�b�N������炷
				DecStackObject( &Stack ) ;

				// �e�I�u�W�F�N�g�̃A�h���X��ύX����
				O = Stack.Stack[Stack.StackNum-1] ;

				// �I�t�Z�b�g��i�܂���
				if( !binf ) PStrMoveP( &PS, "}" ) ;
				else PStrMovePB( &PS, TOKEN_CBRACE ) ;
			}
			else
			{
				// �e���v���[�g���̎擾
				if( !binf )
				{
					if( _STRCHR( StrB, '{' ) != NULL  )
					{
						PStrGetNMP( &PS, StrB, sizeof( StrB ), "{" ) ;

						// ��������ƕ�������擾����
						PStrGet( &PS, StrB, sizeof( StrB ), "{" ) ;
					}
					else
					{
						// ��������ƕ�������擾����
						PStrGet( &PS, StrB, sizeof( StrB ) ) ;
					}
				}
				else
				{
					PStrGetNameBP( &PS, StrB, sizeof( StrB ) ) ;
				}

				// �e��e���v���[�g������Ɣ�r����
				for( Temp = 0 ; Temp < TEMP_NUM ; Temp ++ )
				{
					if( TTable[Temp][0] != '\0' && _STRICMP( StrB, TTable[Temp] ) == 0 ) break ;
				}

				// �����Ή����Ă��Ȃ��e���v���[�g�������ꍇ�͎��́w{...}�x�͔�΂�
				if( Temp == TEMP_NUM )
				{
					if( !binf ) PStrKakkoSkip( &PS ) ;
					else PStrKakkoSkipB( &PS ) ;
				}
				else
				{
					// ���̕�����̎擾
					if( !binf )
					{
						PStrGetNMP( &PS, Name, sizeof( Name ) ) ;
					}
					else
					{
						if( PSTRWORD(&PS,0) == TOKEN_NAME )
						{
							PStrGetNameB( &PS, Name, sizeof( Name ) ) ;
						}
						else
						{
							if( PSTRWORD(&PS,0) == TOKEN_OBRACE ) _STRCPY_S( Name, sizeof( Name ), "{" ) ;
							else Name[0] = '\0' ;
						}
					}

					// �������O�ł͂Ȃ�������m�t�k�k�������Z�b�g
					if( Name[0] == '{' )
					{
						Name[0] = '\0' ;
					}
					else
					{
						if( !binf )
						{
							if( _STRCHR( Name, '{' ) != NULL )
							{
								PStrGetNMP( &PS, Name, sizeof( Name ), "{" ) ;
							}
						}
					}

					// �V�����I�u�W�F�N�g�̍쐬
					O = AddObject( Temp, Name, Stack.Stack[Stack.StackNum-1], Model ) ;
					if( O == NULL )
					{
						DXST_ERRORLOGFMT_ADDA( ( "Load XFile : \x83\x49\x83\x75\x83\x57\x83\x46\x83\x4e\x83\x67 %s ( %s ) \x82\xcc\x92\xc7\x89\xc1\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd"/*@ "Load XFile : �I�u�W�F�N�g %s ( %s ) �̒ǉ��Ɏ��s���܂���" @*/, Name, TTable[ Temp ] ) ) ;
						return -1 ;
					}

					// �X�^�b�N�ɐς�
					if( IncStackObject( &Stack, O ) == -1 )
					{
						DXST_ERRORLOGFMT_ADDA( ( "Load XFile : \x83\x49\x83\x75\x83\x57\x83\x46\x83\x4e\x83\x67 %s ( %s ) \x82\xcc\x83\x6c\x83\x58\x83\x67\x82\xaa\x90\x5b\x82\xb7\x82\xac\x82\xdc\x82\xb7"/*@ "Load XFile : �I�u�W�F�N�g %s ( %s ) �̃l�X�g���[�����܂�" @*/, Name, TTable[ Temp ] ) ) ;
						return -1 ;
					}

					// �����ʂ̒��ɓ���
					if( !binf ) PStrMoveP( &PS, "{" ) ;
					else PStrMovePB( &PS, TOKEN_OBRACE ) ;

					// Temp �̒l�ɂ���ď����𕪊�
					switch( Temp )
					{
					case TEMP_MATERIAL :				Ret = TempMaterial(             &PS, Model, ( X_MATERIAL             * )DOFF( O ) ) ; break ;	// �}�e���A��
					case TEMP_TEXTUREFILENAME :			Ret = TempTextureFilename(      &PS, Model, ( X_TEXTUREFILENAME      * )DOFF( O ) ) ; break ;	// �e�N�X�`���t�@�C���l�[��
					case TEMP_MESHFACE :				Ret = TempMeshFace(             &PS, Model, ( X_MESHFACE             * )DOFF( O ) ) ; break ;	// ���b�V���t�F�C�X
					case TEMP_MESHTEXTURECOORDS :		Ret = TempMeshTextureCoords(    &PS, Model, ( X_MESHTEXTURECOORDS    * )DOFF( O ) ) ; break ;	// ���b�V���e�N�X�`�����W
					case TEMP_MESHNORMALS :				Ret = TempMeshNormals(          &PS, Model, ( X_MESHNORMALS          * )DOFF( O ) ) ; break ;	// ���b�V���̖@��
					case TEMP_MESHMATERIALLIST :		Ret = TempMeshMaterialList(     &PS, Model, ( X_MESHMATERIALLIST     * )DOFF( O ) ) ; break ;	// ���b�V���}�e���A�����X�g
					case TEMP_INDEXEDCOLOR :			Ret = TempIndexedColor(			&PS, Model, ( X_INDEXEDCOLOR         * )DOFF( O ) ) ; break ;	// �C���f�b�N�X�J���[
					case TEMP_MESHVERTEXCOLORS :		Ret = TempMeshVertexColors(		&PS, Model, ( X_MESHVERTEXCOLORS     * )DOFF( O ) ) ; break ;	// ���b�V�����_�J���[
					case TEMP_MESH :					Ret = TempMesh(                 &PS, Model, ( X_MESH                 * )DOFF( O ) ) ; break ;	// ���b�V��
					case TEMP_FRAMETRANSFORMMATRIX :	Ret = TempFrameTransformMatrix( &PS, Model, ( X_FRAMETRANSFORMMATRIX * )DOFF( O ) ) ; break ;	// �t���[���̃��[�J���g�����X�t�H�[��
					case TEMP_FRAME :					Ret = TempFrame(                &PS, Model, ( X_FRAME                * )DOFF( O ) ) ; break ;	// �t���[��
					case TEMP_FLOATKEYS :				Ret = TempFloatKeys(            &PS, Model, ( X_FLOATKEYS            * )DOFF( O ) ) ; break ;	// ���������_���̔z��y�єz����� float �̐�
					case TEMP_TIMEDFLOATKEYS :			Ret = TempTimedFloatKeys(       &PS, Model, ( X_TIMEDFLOATKEYS       * )DOFF( O ) ) ; break ;	// �A�j���[�V�����Ŏg�p����镂�������_���̃Z�b�g�y�ѐ��̎���
					case TEMP_ANIMATIONKEY :			Ret = TempAnimationKey(         &PS, Model, ( X_ANIMATIONKEY         * )DOFF( O ) ) ; break ;	// �A�j���[�V�����L�[�̃Z�b�g
					case TEMP_ANIMATIONOPTIONS :		Ret = TempAnimationOptions(     &PS, Model, ( X_ANIMATIONOPTIONS     * )DOFF( O ) ) ; break ;	// �A�j���[�V�����I�v�V����
					case TEMP_ANIMATION :				Ret = TempAnimation(            &PS, Model, ( X_ANIMATION            * )DOFF( O ) ) ; break ;	// �O�̃t���[�����Q�Ƃ���A�j���[�V�������i�[����
					case TEMP_ANIMATIONSET :			Ret = TempAnimationSet(         &PS, Model, ( X_ANIMATIONSET         * )DOFF( O ) ) ; break ;	// Animation �I�u�W�F�N�g���i�[����
					case TEMP_XSKINMESHHEADER :			Ret = TempXSkinMeshHeader(      &PS, Model, ( X_XSKINMESHHEADER      * )DOFF( O ) ) ; break ;	// �X�L�j���O���b�V���̏��
					case TEMP_SKINWEIGHTS :				Ret = TempSkinWeights(          &PS, Model, ( X_SKINWEIGHTS          * )DOFF( O ) ) ; break ;	// �X�L�����b�V���̃E�G�C�g
					}
					if( Ret == -1 )
					{
						DXST_ERRORLOGFMT_ADDA( ( "Load XFile : \x83\x49\x83\x75\x83\x57\x83\x46\x83\x4e\x83\x67 %s ( %s ) \x89\xf0\x90\xcd\x92\x86\x82\xc9\x83\x47\x83\x89\x81\x5b\x82\xaa\x94\xad\x90\xb6\x82\xb5\x82\xdc\x82\xb5\x82\xbd"/*@ "Load XFile : �I�u�W�F�N�g %s ( %s ) ��͒��ɃG���[���������܂���" @*/, Name, TTable[ Temp ] ) ) ;
						return -1 ;
					}
				}
			}
		}
	}

	// �ԐڎQ�Ƃ̃I�u�W�F�N�g���m�̊֘A�t�����s��
	for( O = M->FirstObject ; O ; O = O->NextData )
	{
		// �ԐڎQ�Ƃ̐������J��Ԃ�
		for( i = 0 ; i < O->RelationObjectNum ; i ++ )
		{
//			// ���ɎQ�Ɛ�̏�񂪂���ꍇ�͉������Ȃ�
//			if( O->RelationObject[ i ] ) continue ;

			// ���O�������ꍇ�͉������Ȃ�
			if( O->RelationObject[ i ].ObjectName == NULL || O->RelationObject[ i ].ObjectName[ 0 ] == '\0' )
			{
				continue ;
			}

			// �w��̃I�u�W�F�N�g�Ɠ������O�̃I�u�W�F�N�g��T��
			for( Ob = M->FirstObject ; Ob ; Ob = Ob->NextData )
			{
				void *OldRelationObject ;
				int NewMaxNum ;

				if( _STRCMP( Ob->Name, O->RelationObject[ i ].ObjectName ) != 0 )
				{
					continue ;
				}

				// �������I�u�W�F�N�g�̃A�h���X��ۑ�
				if( O->RelationObject[ i ].ObjectMaxNum == O->RelationObject[ i ].ObjectNum )
				{
					OldRelationObject = O->RelationObject[ i ].Object ;
					NewMaxNum = O->RelationObject[ i ].ObjectMaxNum + 32 ;
					O->RelationObject[ i ].Object = ( X_OBJECT ** )ADDMEMAREA( sizeof( X_OBJECT * ) * NewMaxNum, &M->XModelMem ) ;
					if( O->RelationObject[ i ].Object == NULL )
						return DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x93\x95\xa5\x63\xc2\x53\x67\x71\x28\x75\xdd\x30\xa4\x30\xf3\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �ԐڎQ�Ɨp�|�C���^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					if( O->RelationObject[ i ].ObjectMaxNum != 0 )
					{
						_MEMCPY( O->RelationObject[ i ].Object, OldRelationObject, sizeof( X_OBJECT * ) * O->RelationObject[ i ].ObjectMaxNum ) ;
					}
					O->RelationObject[ i ].ObjectMaxNum = NewMaxNum ;
				}
				O->RelationObject[ i ].Object[ O->RelationObject[ i ].ObjectNum ] = Ob ;
				O->RelationObject[ i ].ObjectNum ++ ;

				// �����̃I�u�W�F�N�g�Ɗ֘A�t��������
				if( Ob->RelationObjectMaxNum == Ob->RelationObjectNum )
				{
					OldRelationObject = Ob->RelationObject ;
					NewMaxNum = Ob->RelationObjectMaxNum + 32 ;
					Ob->RelationObject = ( X_RELATION_OBJECT * )ADDMEMAREA( sizeof( X_RELATION_OBJECT ) * NewMaxNum, &M->XModelMem ) ;
					if( Ob->RelationObject == NULL )
						return DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x93\x95\xa5\x63\xc2\x53\x67\x71\x28\x75\xdd\x30\xa4\x30\xf3\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �ԐڎQ�Ɨp�|�C���^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					if( Ob->RelationObjectMaxNum != 0 )
					{
						_MEMCPY( Ob->RelationObject, OldRelationObject, sizeof( X_RELATION_OBJECT ) * Ob->RelationObjectMaxNum ) ;
					}
					Ob->RelationObjectMaxNum = NewMaxNum ;
/*
					void *OldRelationObject, *OldRelationObjectName ;
					int NewMaxNum ;

					OldRelationObject = Ob->RelationObject ;
					OldRelationObjectName = Ob->RelationObjectName ;
					NewMaxNum = Ob->RelationObjectMaxNum + 32 ;
					Ob->RelationObject = ( X_OBJECT ** )ADDMEMAREA( ( sizeof( X_OBJECT * ) + sizeof( char * ) ) * NewMaxNum, &M->XModelMem ) ;
					if( Ob->RelationObject == NULL )
						return DXST_ERRORLOGFMT_ADDW(( L"Load XFile : �ԐڎQ�Ɨp�|�C���^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" )) ;
					Ob->RelationObjectName = ( char ** )( Ob->RelationObject + NewMaxNum ) ;
					if( Ob->RelationObjectMaxNum != 0 )
					{
						_MEMCPY( Ob->RelationObject,     OldRelationObject,     sizeof( X_OBJECT * ) * Ob->RelationObjectMaxNum ) ;
						_MEMCPY( Ob->RelationObjectName, OldRelationObjectName, sizeof( char * )     * Ob->RelationObjectMaxNum ) ;
					}
					Ob->RelationObjectMaxNum = NewMaxNum ;
*/
				}

				NewMaxNum = 1 ;
				Ob->RelationObject[ Ob->RelationObjectNum ].Object = ( X_OBJECT ** )ADDMEMAREA( sizeof( X_OBJECT * ) * NewMaxNum, &M->XModelMem ) ;
				if( Ob->RelationObject[ Ob->RelationObjectNum ].Object == NULL )
					return DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x93\x95\xa5\x63\xc2\x53\x67\x71\x28\x75\xdd\x30\xa4\x30\xf3\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �ԐڎQ�Ɨp�|�C���^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
				Ob->RelationObject[ Ob->RelationObjectNum ].ObjectMaxNum = NewMaxNum ;
				Ob->RelationObject[ Ob->RelationObjectNum ].ObjectName = ( char * )"" ;
				Ob->RelationObject[ Ob->RelationObjectNum ].Object[ 0 ] = O ;
				Ob->RelationObject[ Ob->RelationObjectNum ].ObjectNum ++ ;
//				Ob->RelationObject[ Ob->RelationObjectNum ] = O ;
				Ob->RelationObjectNum ++ ;
			}
/*
			// �w��̃I�u�W�F�N�g�Ɠ������O�̃I�u�W�F�N�g��T��
			for( Ob = M->FirstObject ; Ob && _STRCMP( Ob->Name, O->RelationObjectName[ i ] ) != 0 ; Ob = Ob->NextData ){}

			// �w��̃I�u�W�F�N�g���Ȃ������牽�����Ȃ�
			if( Ob == NULL ) 
			{
				DXST_ERRORLOG_ADDW( L"Load XFile : �ԐڎQ�Ƃ����s���܂���\n" ) ;
			}	
			else
			{
				// �������I�u�W�F�N�g�̃A�h���X��ۑ�
				O->RelationObject[ i ] = Ob ;

				// �����̃I�u�W�F�N�g�Ɗ֘A�t��������
				if( Ob->RelationObjectMaxNum == Ob->RelationObjectNum )
				{
					void *OldRelationObject, *OldRelationObjectName ;
					int NewMaxNum ;

					OldRelationObject = Ob->RelationObject ;
					OldRelationObjectName = Ob->RelationObjectName ;
					NewMaxNum = Ob->RelationObjectMaxNum + 32 ;
					Ob->RelationObject = ( X_OBJECT ** )ADDMEMAREA( ( sizeof( X_OBJECT * ) + sizeof( char * ) ) * NewMaxNum, &M->XModelMem ) ;
					if( Ob->RelationObject == NULL )
						return DXST_ERRORLOGFMT_ADDW(( L"Load XFile : �ԐڎQ�Ɨp�|�C���^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" )) ;
					Ob->RelationObjectName = ( char ** )( Ob->RelationObject + NewMaxNum ) ;
					if( Ob->RelationObjectMaxNum != 0 )
					{
						_MEMCPY( Ob->RelationObject,     OldRelationObject,     sizeof( X_OBJECT * ) * Ob->RelationObjectMaxNum ) ;
						_MEMCPY( Ob->RelationObjectName, OldRelationObjectName, sizeof( char * )     * Ob->RelationObjectMaxNum ) ;
					}
					Ob->RelationObjectMaxNum = NewMaxNum ;
				}
				Ob->RelationObject[ Ob->RelationObjectNum ] = O ;
				Ob->RelationObjectNum ++ ;
			}
*/
		}
	}

	// �I��
	return 0 ;
}

// �w�t�@�C���̃��f���f�[�^����͂��A���s�p�f�[�^�̏������s��
extern int AnalysXData( X_MODEL *Model, MV1_MODEL_R *RModel )
{
	X_OBJECT *O/*, *BO*/ ;
	X_MODEL *M ;
	int i, j, k, l ;

	M = Model ;
	if( M == NULL ) return -1 ;

	// ���ׂẴI�u�W�F�N�g�𑖍�����
	O = M->FirstObject ;
	while( O != NULL )
	{
		// �I�u�W�F�N�g�̃^�C�v�ɂ���ď����𕪊�
		switch( O->Type )
		{
		case TEMP_MATERIAL :			// �}�e���A��
			{
				X_MATERIAL *MT = ( X_MATERIAL * )DOFF( O ) ;

				// ���̃}�e���A���Ɋ֘A�t�����Ă���e�N�X�`���[�t�@�C���l�[�����擾����
				MT->TextureFileName = ( X_TEXTUREFILENAME * )GetRelationObject( TEMP_TEXTUREFILENAME, O ) ;

				// �e�N�X�`�������������ꍇ�ŁA���ǂ����̃I�u�W�F�N�g�ɏ�������ꍇ�͒��O�̃I�u�W�F�N�g�̃}�e���A���̃e�N�X�`���t�@�C�����g�p����
/*				if( MT->TextureFileName == NULL && O->ParentsData->ParentsData != NULL )
				{
					BO = GetPrevObject( M, O ) ;
					if( BO && BO->Type == TEMP_MATERIAL && ( ( X_MATERIAL * )DOFF( BO ) )->TextureFileName )
					{
						MT->TextureFileName = ( ( X_MATERIAL * )DOFF( BO ) )->TextureFileName ;
					}
				}
*/
				// �}�e���A���̃C���f�b�N�X���Z�b�g
				MT->Index = M->MaterialNum ;

				// �}�e���A���̐����C���N�������g
				M->MaterialNum ++ ;
			}
			break ;

		case TEMP_MESHMATERIALLIST :	// ���b�V���}�e���A�����X�g
			{
				X_MESHMATERIALLIST *ML = ( X_MESHMATERIALLIST * )DOFF( O ) ;
				DWORD Num ;

				// �֘A�t�����Ă���}�e���A���̃A�h���X���i�[���邽�߂̃������̈���m��
				if( ( ML->MaterialList = ( X_MATERIAL ** )ADDMEMAREA( sizeof( X_MATERIAL *) * ML->nMaterials, &M->XModelMem ) ) == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xc3\x30\xb7\x30\xe5\x30\x6b\x30\xa2\x95\x23\x90\xd8\x4e\x51\x30\x89\x30\x8c\x30\x66\x30\x44\x30\x8b\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���b�V���Ɋ֘A�t�����Ă���}�e���A�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
					return -1 ;
				}

				// �֘A�t�����Ă���}�e���A�����擾����
				EnumRelationObject( TEMP_MATERIAL, ( int * )&Num, ( void ** )ML->MaterialList, O ) ;

				// �����������G���[
				if( Num != ML->nMaterials )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xea\x30\xb9\x30\xc8\x30\x6e\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x70\x65\x28\x00\x20\x00\x25\x00\x64\x00\x20\x00\x29\x00\x68\x30\x9f\x5b\x9b\x96\x6e\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x6e\x30\x70\x65\x28\x00\x20\x00\x25\x00\x64\x00\x20\x00\x29\x00\x4c\x30\x55\x90\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �}�e���A�����X�g�̃}�e���A����( %d )�Ǝ��ۂ̃}�e���A���̐�( %d )���Ⴂ�܂���\n" @*/, ML->nMaterials, Num ) ) ;
					return -1 ;
				}
			}
			break ;

		case TEMP_MESH :				// ���b�V��
			{
				X_MESH *MS = ( X_MESH * )DOFF( O ) ;

				// ���b�V���Ɋ֘A�t�����Ă��钸�_�J���[�𓾂�
				MS->VertexColor = ( X_MESHVERTEXCOLORS * )GetRelationObject( TEMP_MESHVERTEXCOLORS, O ) ;

				// ���b�V���Ɋ֘A�t�����Ă���e�N�X�`���[���W�𓾂�
				MS->TextureCoord = ( X_MESHTEXTURECOORDS * )GetRelationObject( TEMP_MESHTEXTURECOORDS, O ) ;

				// �e�N�X�`�����W�ƒ��_���W�̐����Ⴄ�ꍇ�̓G���[
				if( MS->TextureCoord && MS->TextureCoord->nTextureCoords != MS->nVertices )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xa7\x5e\x19\x6a\x6e\x30\x70\x65\x28\x00\x20\x00\x25\x00\x64\x00\x20\x00\x29\x00\x68\x30\x02\x98\xb9\x70\xa7\x5e\x19\x6a\x6e\x30\x70\x65\x28\x00\x20\x00\x25\x00\x64\x00\x20\x00\x29\x00\x4c\x30\x55\x90\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �e�N�X�`�����W�̐�( %d )�ƒ��_���W�̐�( %d )���Ⴂ�܂���\n" @*/, MS->TextureCoord->nTextureCoords, MS->nVertices ) ) ;
					return -1 ;
				}

				// ���b�V���Ɋ֘A�t�����Ă���@���f�[�^�𓾂�
				MS->Normals = ( X_MESHNORMALS * )GetRelationObject( TEMP_MESHNORMALS, O ) ;

				// ���b�V���Ɋ֘A�t�����Ă���}�e���A�����X�g�𓾂�
				MS->MaterialList = ( X_MESHMATERIALLIST * )GetRelationObject( TEMP_MESHMATERIALLIST, O ) ;

				// �ʂ̐����������G���[
				if( MS->Normals && MS->nFaces != MS->Normals->nFaceNormals )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xea\x30\xb9\x30\xc8\x30\x6e\x30\x62\x97\x6e\x30\x70\x65\x28\x00\x20\x00\x25\x00\x64\x00\x20\x00\x29\x00\x68\x30\xd5\x6c\xda\x7d\x6e\x30\x62\x97\x6e\x30\x70\x65\x28\x00\x20\x00\x25\x00\x64\x00\x20\x00\x29\x00\x4c\x30\x55\x90\x44\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���b�V�����X�g�̖ʂ̐�( %d )�Ɩ@���̖ʂ̐�( %d )���Ⴂ�܂���\n" @*/, MS->nFaces, MS->Normals->nFaceNormals ) ) ;
					return -1 ;
				}

				// ���b�V���Ɋ֘A�t�����Ă���X�L�����b�V���̃E�G�C�g�l�̐��𓾂�
				EnumRelationObject( TEMP_SKINWEIGHTS, &MS->SkinWeightsNum, NULL, O ) ;

				// �E�G�C�g�l�f�[�^�̃��X�g���i�[���郁�����̈���m��
				if( MS->SkinWeightsNum )
				{
					if( ( MS->SkinWeights = ( X_SKINWEIGHTS ** )ADDMEMAREA( sizeof( X_SKINWEIGHTS * ) * MS->SkinWeightsNum, &M->XModelMem ) ) == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xf3\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �X�L���E�G�C�g�f�[�^���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
						return -1 ;
					}

					// �ēx���b�V���Ɋ֘A�t�����Ă���X�L�����b�V���̃E�G�C�g�l�̃f�[�^�̃A�h���X�𓾂�
					EnumRelationObject( TEMP_SKINWEIGHTS, &MS->SkinWeightsNum, ( void ** )MS->SkinWeights, O ) ;
				}
				else
				{
					MS->SkinWeights = NULL ;
				}

				// �g���C�A���O���̐������Z
				M->TriangleNum += MS->nTriangles ;

				// �X�L�����b�V���̃E�G�C�g�Ɋւ�����̐������Z
				M->SkinWeightNum += MS->SkinWeightsNum ;

				// ���b�V���̐����C���N�������g
				M->MeshNum ++ ;
			}
			break ;

		case TEMP_FRAME :				// �t���[��
			{
				X_FRAME *Frame = ( X_FRAME * )DOFF( O ) ;
				X_MESH *MS ;
				X_OBJECT *Ob ;

				// ���̃t���[���Ɋ֘A�t�����Ă���g�����X�t�H�[���s��̎擾
				Frame->TransformMatrix = ( X_FRAMETRANSFORMMATRIX * )GetRelationObject( TEMP_FRAMETRANSFORMMATRIX, O ) ;

				// ���̃t���[���Ɋ֘A�t�����Ă��郁�b�V���Ƀ��b�V���̍��W��o�^����
				Ob = O->ChildFirstData ;
				Frame->MeshNum = 0 ;
				while( Ob != NULL )
				{
					if( Ob->Type == TEMP_MESH )
					{
						MS = ( X_MESH * )DOFF( Ob ) ;
						MS->Frame = Frame ;
						Frame->MeshNum ++ ;
					}
					Ob = Ob->ChildNextData ;
				}

				// �֘A�t�����Ă��郁�b�V���̃��X�g���쐬����
				if( ( Frame->Mesh = ( X_MESH ** )ADDMEMAREA( sizeof( X_MESH * ) * Frame->MeshNum, &M->XModelMem ) ) == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x4d\x91\x17\x52\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���b�V���A�h���X�z����i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
					return -1 ;
				}
				EnumRelationObject( TEMP_MESH, ( int * )&Frame->MeshNum, ( void ** )Frame->Mesh, O ) ;

				// �t���[���̃C���f�b�N�X���Z�b�g
				Frame->Index = M->FrameNum ;

				// �t���[���̐����C���N�������g
				M->FrameNum ++ ;
			}
			break ;

		case TEMP_ANIMATION :			// �A�j���[�V����
			{
				DWORD Max ;
				X_ANIMATION *AN = ( X_ANIMATION * )DOFF( O ) ;

				// �A�j���[�V�����L�[�̐��𐔂���
				EnumRelationObject( TEMP_ANIMATIONKEY, ( int * )&AN->AnimationKeysNum, NULL, O ) ;
				
				// �A�j���[�V�����L�[�̃A�h���X��ۑ����郁�����̈�̊m��
				if( ( AN->AnimationKeys = ( X_ANIMATIONKEY ** )ADDMEMAREA( sizeof( X_ANIMATIONKEY * ) * AN->AnimationKeysNum, &M->XModelMem ) ) == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x4d\x91\x17\x52\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �A�j���[�V�����L�[�A�h���X�z����i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
					return -1 ;
				}

				// �A�j���[�V�����L�[�̃A�h���X���擾����
				EnumRelationObject( TEMP_ANIMATIONKEY, ( int * )&AN->AnimationKeysNum, ( void ** )AN->AnimationKeys, O ) ;

				// �ő�J�E���g�𓾂�
				Max = 0 ;
				for( i = 0 ; ( DWORD )i < AN->AnimationKeysNum ; i ++ )
				{
					AN->AnimationKeys[ i ]->TotalCount = ( int )( AN->AnimationKeys[ i ]->keys[ AN->AnimationKeys[ i ]->nKeys - 1 ].time ) ;
					if( Max < ( DWORD )AN->AnimationKeys[ i ]->TotalCount ) Max = ( DWORD )AN->AnimationKeys[ i ]->TotalCount ;
				}
				AN->MaxCount = ( int )Max ;

				// �A�j���[�V�����f�[�^�Ɋ֘A���Ă���t���[���̃f�[�^�A�h���X�𓾂�
				AN->Frame = ( X_FRAME * )GetRelationObject( TEMP_FRAME, O ) ;
			}
			break ;

		case TEMP_SKINWEIGHTS :			// �X�L�����b�V���̃E�G�C�g
			{
				X_SKINWEIGHTS *SW = ( X_SKINWEIGHTS * )DOFF( O ) ;

				// �w��̃t���[�����ɍ��v����I�u�W�F�N�g���擾����
				SW->Frame = ( X_FRAME * )GetMatchNameObject( TEMP_FRAME, SW->transformNodeName.String, Model ) ;
			}
			break ;
		}

		O = O->NextData ;
	}

	// ������x���ׂẴI�u�W�F�N�g�𑖍�����
	O = M->FirstObject ;
	while( O != NULL )
	{
		// �I�u�W�F�N�g�̃^�C�v�ɂ���ď����𕪊�
		switch( O->Type )
		{
		case TEMP_ANIMATIONSET :		// �A�j���[�V�����Z�b�g
			{
				int Max, Num ;

				X_ANIMATIONSET *AS = ( X_ANIMATIONSET * )DOFF( O ) ;
				X_ANIMATION *AN ;
				X_ANIMATIONKEY *AK ;

				// �A�j���[�V�����̐��𐔂���
				EnumRelationObject( TEMP_ANIMATION, &AS->AnimationNum, NULL, O ) ;
				
				// �A�j���[�V�����̃A�h���X��ۑ����郁�����̈�̊m��
				if( ( AS->Animation = ( X_ANIMATION ** )ADDMEMAREA( sizeof( X_ANIMATION * ) * AS->AnimationNum, &M->XModelMem ) ) == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x4d\x91\x17\x52\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �A�j���[�V�����A�h���X�z����i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
					return -1 ;
				}

				// �A�j���[�V�����̃A�h���X���擾����
				EnumRelationObject( TEMP_ANIMATION, &AS->AnimationNum, ( void ** )AS->Animation, O ) ;

				// �A�j���[�V�����̍ő�A�j���[�V�����J�E���g�l�𓾂�
				// ���łɗL���ȃA�j���[�V�����̐��𐔂���
				Max = 0 ;
				Num = 0 ;
				for( i = 0 ; i < AS->AnimationNum ; i ++ )
				{
					if( AS->Animation[i]->Frame == NULL ) continue ;
					Num ++ ;
					if( Max < AS->Animation[i]->MaxCount ) Max = AS->Animation[i]->MaxCount ;
				}
				AS->MaxCount = Max ;

				// �e�f�[�^�̐����J�E���g����
				for( i = 0 ; i < AS->AnimationNum ; i ++ )
				{
					AN = AS->Animation[ i ] ;

					if( AS->Animation[i]->Frame == NULL ) continue ;

					// �֘A����t���[���̖��O��ۑ����邽�߂̃������T�C�Y�����Z
					Model->StringSize += ( int )_STRLEN( OOFF( AN->Frame )->Name ) + 1 ;
					Model->StringSize = ( Model->StringSize + 3 ) / 4 * 4 ;

					// �A�j���[�V�����L�[�Z�b�g�̐������Z����
					Model->AnimKeySetNum += AN->AnimationKeysNum ;

					// �e�L�[�̗v�f�̐������Z����
					for( j = 0 ; ( DWORD )j < AN->AnimationKeysNum ; j ++ )
					{
						DWORD DataSize = 0 ;
						AK = AN->AnimationKeys[ j ] ;

						// �A�j���[�V�����L�[�̐������Z����
						switch( AK->keyType )
						{
						case 0 : DataSize = AK->nKeys * ( sizeof( FLOAT4                  ) + sizeof( float ) ) ; break ;
						case 1 : DataSize = AK->nKeys * ( sizeof( VECTOR                  ) + sizeof( float ) ) ; break ;
						case 2 : DataSize = AK->nKeys * ( sizeof( VECTOR                  ) + sizeof( float ) ) ; break ;
						case 4 : DataSize = AK->nKeys * ( sizeof( MV1_ANIM_KEY_MATRIX4X4C ) + sizeof( float ) ) ; break ;
						}
						Model->AnimKeyDataSize += DataSize ;
						AS->KeyDataSize += DataSize ;
					}
				}

				// �A�j���[�V�����̐������Z
				Model->AnimNum += Num ;

				// �A�j���[�V�����Z�b�g�̐����C���N�������g
				Model->AnimSetNum ++ ;
			}
			break ;
		}

		O = O->NextData ;
	}

	// �ǂݍ��݃��f���̍\�z
	{
		MV1_MATERIAL_R *Material ;
		MV1_TEXTURE_R *Texture ;
		MV1_ANIMSET_R *AnimSet ;
		MV1_ANIM_R *Anim ;
		MV1_ANIMKEYSET_R *AnimKeySet ;
		MV1_FRAME_R *Frame ;
		MV1_MESH_R *Mesh ;
		MV1_SKIN_WEIGHT_R *SkinWeight ;
		MV1_SKIN_WEIGHT_ONE_R *SkinWeightOne ;

		// �}�e���A���ƃe�N�X�`���̒ǉ�
		for( O = M->FirstObject ; O ; O = O->NextData )
		{
			// �}�e���A���ł͂Ȃ��ꍇ�̓X�L�b�v
			if( O->Type != TEMP_MATERIAL ) continue ;

			X_MATERIAL *MT = ( X_MATERIAL * )DOFF( O ) ;

			// �}�e���A���̒ǉ�
			Material = MV1RAddMaterial( RModel, O->Name ) ;
			if( Material == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �}�e���A���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
				return -1 ;
			}
			
			// �e�N�X�`�����g�p���Ă���ꍇ�̓e�N�X�`�����ǉ�
			if( MT->TextureFileName )
			{
				Texture = MV1RAddTexture( RModel, MT->TextureFileName->filename.String, MT->TextureFileName->filename.String ) ;
				if( Texture == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �e�N�X�`���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
					return -1 ;
				}

				// �p�����[�^�̃Z�b�g
				Material->DiffuseTexNum = 1 ;
				Material->DiffuseTexs[ 0 ] = Texture ;
			}

			// �p�����[�^�̃Z�b�g
			Material->Ambient.r = 0.0f ;
			Material->Ambient.g = 0.0f ;
			Material->Ambient.b = 0.0f ;
			Material->Ambient.a = 0.0f ;
			Material->Diffuse.r = MT->faceColor.red ;
			Material->Diffuse.g = MT->faceColor.green ;
			Material->Diffuse.b = MT->faceColor.blue ;
			Material->Diffuse.a = 1.0f ;
			Material->Specular.r = MT->specularColor.red ;
			Material->Specular.g = MT->specularColor.green ;
			Material->Specular.b = MT->specularColor.blue ;
			Material->Specular.a = 0.0f ;
			Material->Emissive.r = MT->emissiveColor.red ;
			Material->Emissive.g = MT->emissiveColor.green ;
			Material->Emissive.b = MT->emissiveColor.blue ;
			Material->Emissive.a = 0.0f ;
			Material->Power = MT->power ;

			// faceColor �� alpha �� 1.0f �ӊO�������ꍇ�͕`�惂�[�h�̃p�����[�^��ύX����
			if( MT->faceColor.alpha < 0.9999999f )
			{
				Material->DrawBlendParam = _FTOL( MT->faceColor.alpha * 255.0f ) ;
				if( Material->DrawBlendParam < 0 )
				{
					Material->DrawBlendParam = 0 ;
				}
				if( Material->DrawBlendParam > 255 )
				{
					Material->DrawBlendParam = 255 ;
				}
			}

			MT->MV1RMaterial = Material ;
		}

		// �t���[���̒ǉ�
		for( O = M->FirstObject ; O ; O = O->NextData )
		{
			// �e�����Ȃ��ꍇ�̓X�L�b�v
			if( O->ParentsData == NULL ) continue ;

			// �t���[���I�u�W�F�N�g�ł͂Ȃ��ꍇ�̓X�L�b�v
			if( O->Type != TEMP_FRAME ) continue ;

			X_FRAME *FR = ( X_FRAME * )DOFF( O ) ;

			// �t���[���̒ǉ�
			Frame = MV1RAddFrame( RModel, O->Name, O->ParentsData && O->ParentsData->ParentsData && O->ParentsData->Type == TEMP_FRAME ? ( ( X_FRAME * )DOFF( O->ParentsData ) )->MV1Frame : NULL ) ;
			if( Frame == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xd5\x30\xec\x30\xfc\x30\xe0\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �t���[���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
				return -1 ;
			}

			// �p�����[�^�̃Z�b�g
			FR->MV1Frame = Frame ; 

			// �s��̃Z�b�g
			if( FR->TransformMatrix != NULL )
			{
				Frame->Matrix = FR->TransformMatrix->frameMatrix.ms ;
			}
			else
			{
				CreateIdentityMatrix( &Frame->Matrix ) ;
			}

			// ���b�V��������ꍇ�̓��b�V���̒ǉ�
			for( i = 0 ; i < FR->MeshNum ; i ++ )
			{
				X_MESH *MS ;

				MS = FR->Mesh[ i ] ;

				// ���b�V���ɖ��O�����邩�A�e�̋��Ȃ����b�V���������ꍇ�̓��b�V�����Ńt���[�����쐬����
				if( OOFF( MS )->Name[ 0 ] != '\0' || OOFF( MS )->ParentsData == NULL || OOFF( MS )->ParentsData->ParentsData == NULL )
				{
					MS->MeshFrame = MV1RAddFrame( RModel, OOFF( MS )->Name, Frame ) ;
				}
			}
		}

		// ���b�V���ƃA�j���[�V�����̒ǉ�
		O = M->FirstObject ;
		while( O != NULL )
		{
			// �I�u�W�F�N�g�̃^�C�v�ɂ���ď����𕪊�
			switch( O->Type )
			{
			case TEMP_FRAME :		// �t���[��
				X_FRAME *FR ;
				X_MESH *MS ;
				X_MESHFACE *MF ;
				X_SKINWEIGHTS *SW ;
				X_INDEXEDCOLOR *IC ;

				FR = ( X_FRAME * )DOFF( O ) ;

				// �t���[���̃A�h���X�Z�b�g
				Frame = FR->MV1Frame ;

				// ���b�V��������ꍇ�̓��b�V���̒ǉ�
				for( i = 0 ; i < FR->MeshNum ; i ++ )
				{
					MS = FR->Mesh[ i ] ;

					// �t���[���̃Z�b�g
					Frame = MS->MeshFrame == NULL ? FR->MV1Frame : MS->MeshFrame ;
					if( Frame == NULL && MS->MeshFrame == NULL )
					{
						MS->MeshFrame = MV1RAddFrame( RModel, OOFF( MS )->Name[ 0 ] == '\0' ? "Mesh" : OOFF( MS )->Name, NULL ) ;
						Frame = MS->MeshFrame ;
					}

					// ���b�V���̒ǉ�
					Mesh = MV1RAddMesh( RModel, Frame ) ;
					if( Mesh == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���b�V���I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						return -1 ;
					}

					// �}�e���A�����̃R�s�[
					if( MS->MaterialList == NULL )
					{
//						DXST_ERRORLOGFMT_ADDW(( L"Load XFile : ���b�V���Ƀ}�e���A���̏�񂪂���܂���\n" )) ;
//						return -1 ;
						// �}�e���A��������������K���ȃ}�e���A����ǉ�����
						Mesh->MaterialNum = 1 ;
						Mesh->Materials[ 0 ] = MV1RAddMaterial( RModel, "Normal" ) ;
					}
					else
					{
						// �g�p���Ă���}�e���A���̏����R�s�[
						Mesh->MaterialNum = MS->MaterialList->nMaterials ;
						for( j = 0 ; j < ( int )Mesh->MaterialNum ; j ++ )
							Mesh->Materials[ j ] = MS->MaterialList->MaterialList[ j ]->MV1RMaterial ;
					}

					// �X�L�j���O���̃R�s�[
					Mesh->SkinWeightsNum = ( DWORD )MS->SkinWeightsNum ;
					for( j = 0 ; j < ( int )Mesh->SkinWeightsNum ; j ++ )
					{
						SW = MS->SkinWeights[ j ] ;
						
						// �X�L�j���O���b�V���̒ǉ�
						SkinWeight = MV1RAddSkinWeight( RModel ) ;
						if( SkinWeight == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xcb\x30\xf3\x30\xb0\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �X�L�j���O���b�V���E�G�C�g�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							return -1 ;
						}
						Mesh->SkinWeights[ j ] = SkinWeight ;

						// �p�����[�^�̃Z�b�g
						SkinWeight->ModelLocalMatrix = SW->matrixOffset.ms ;
						SkinWeight->TargetFrame = ( DWORD )SW->Frame->MV1Frame->Index ;

						// ���_�Ƃ̑Ή��\���i�[���邽�߂̃������̊m��
						SkinWeight->DataNum = SW->nWeights ;
						SkinWeight->Data = ( MV1_SKIN_WEIGHT_ONE_R * )ADDMEMAREA( sizeof( MV1_SKIN_WEIGHT_ONE_R ) * SkinWeight->DataNum, &RModel->Mem ) ;
						if( SkinWeight->Data == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xcb\x30\xf3\x30\xb0\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\x24\x50\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �X�L�j���O���b�V���E�G�C�g�l���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							return -1 ;
						}

						// ���_�Ƃ̑Ή��\���R�s�[����
						SkinWeightOne = SkinWeight->Data ;
						for( k = 0 ; k < ( int )SW->nWeights ; k ++, SkinWeightOne ++ )
						{
							SkinWeightOne->TargetVertex = SW->vertexIndices[ k ] ;
							SkinWeightOne->Weight = SW->weights[ k ] ;
						}
					}

					// ���_�����R�s�[
					Mesh->PositionNum = MS->nVertices ;
					Mesh->Positions = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->PositionNum, &RModel->Mem ) ;
					if( Mesh->Positions == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xa7\x5e\x19\x6a\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���_���W���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
						return -1 ;
					}
					_MEMCPY( Mesh->Positions, MS->vertices, sizeof( VECTOR ) * Mesh->PositionNum ) ;

					// �@�����̃R�s�[
					if( MS->Normals )
					{
						Mesh->NormalNum = MS->Normals->nNormals ;
						Mesh->Normals = ( VECTOR * )ADDMEMAREA( sizeof( VECTOR ) * Mesh->NormalNum, &RModel->Mem ) ;
						if( Mesh->Normals == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xd5\x6c\xda\x7d\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �@�����i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
							return -1 ;
						}
						_MEMCPY( Mesh->Normals, MS->Normals->normals, sizeof( VECTOR ) * Mesh->NormalNum ) ;
					}

					// UV���W�̃R�s�[
					if( MS->TextureCoord )
					{
						Mesh->UVNum[ 0 ] = MS->TextureCoord->nTextureCoords ;
						Mesh->UVs[ 0 ] = ( FLOAT4 * )ADDMEMAREA( sizeof( FLOAT4 ) * Mesh->UVNum[ 0 ], &RModel->Mem ) ;
						if( Mesh->UVs[ 0 ] == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x35\xff\x36\xff\xa7\x5e\x19\x6a\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �t�u���W���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
							return -1 ;
						}
						for( j = 0 ; j < ( int )Mesh->UVNum[ 0 ] ; j ++ )
						{
							Mesh->UVs[ 0 ][ j ].x = MS->TextureCoord->textureCoords[ j ].u ;
							Mesh->UVs[ 0 ][ j ].y = MS->TextureCoord->textureCoords[ j ].v ;
						}
					}

					// �ʏ��̎擾
					{
						// �ʏ����i�[���郁�����̈�̊m��
						if( MV1RSetupMeshFaceBuffer( RModel, Mesh, ( int )MS->nFaces, 4 ) < 0 )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x62\x97\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �ʏ����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							return -1 ;
						}

						// ���_�C���f�b�N�X�ƃ}�e���A���C���f�b�N�X�Ƃt�u�C���f�b�N�X���R�s�[
						MF = MS->faces ;
						for( j = 0 ; j < ( int )Mesh->FaceNum ; j ++, MF ++ )
						{
							Mesh->Faces[ j ].IndexNum = MF->nFaceVertexIndices ;
							if( Mesh->FaceUnitMaxIndexNum < Mesh->Faces[ j ].IndexNum )
							{
								if( MV1RSetupMeshFaceBuffer( RModel, Mesh, ( int )Mesh->FaceNum, ( int )Mesh->Faces[ j ].IndexNum ) < 0 )
								{
									DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x62\x97\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\x8d\x51\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �ʏ����i�[���郁�����̈�̍Ċm�ۂɎ��s���܂���\n" @*/ )) ;
									return -1 ;
								}
							}

							Mesh->Faces[ j ].MaterialIndex = MS->MaterialList == NULL || MS->nFaces != MS->MaterialList->nFaceIndexes ? 0 : MS->MaterialList->FaceIndexes[ j ] ;
							for( k = 0 ; k < ( int )Mesh->Faces[ j ].IndexNum ; k ++ )
							{
								if( MF->faceVertexIndices != NULL )
								{
									Mesh->Faces[ j ].VertexIndex[ k ]  = MF->faceVertexIndices[ k ] ;
									Mesh->Faces[ j ].UVIndex[ 0 ][ k ] = MF->faceVertexIndices[ k ] ;
								}
								else
								{
									Mesh->Faces[ j ].VertexIndex[ k ]  = MF->faceVertexIndicesFixedLength[ k ] ;
									Mesh->Faces[ j ].UVIndex[ 0 ][ k ] = MF->faceVertexIndicesFixedLength[ k ] ;
								}

								if( Mesh->Normals )
								{
									if( MS->Normals->faceNormals[ j ].faceVertexIndices != NULL )
									{
										Mesh->Faces[ j ].NormalIndex[ k ] = MS->Normals->faceNormals[ j ].faceVertexIndices[ k ] ;
									}
									else
									{
										Mesh->Faces[ j ].NormalIndex[ k ] = MS->Normals->faceNormals[ j ].faceVertexIndicesFixedLength[ k ] ;
									}
								}
							}
						}
					}

					// ���_�J���[�̃R�s�[
					if( MS->VertexColor )
					{
						int TotalIndexNum ;
						int *FaceUseVertexIndexBuffer ;
						int *FaceUseVertexIndexList ;
						int *pFUVIB ;
						int *pFUVIL ;
						int FaceUseVertexIndexBufferSize ;

						// ���_�J���[�̐����Z�b�g
						Mesh->VertexColorNum = MS->VertexColor->nVertexColors ;

						// ���_�J���[���i�[���郁�����̈�̊m��
						Mesh->VertexColors   = ( COLOR_F * )ADDMEMAREA( sizeof( COLOR_F ) * ( Mesh->VertexColorNum + 1 ), &RModel->Mem ) ;
						if( Mesh->VertexColors == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xab\x30\xe9\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���_�J���[���i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ )) ;
							return -1 ;
						}

						// �f�t�H���g�̒��_�J���[�p
						Mesh->VertexColors[ Mesh->VertexColorNum ].r = 1.0f ;
						Mesh->VertexColors[ Mesh->VertexColorNum ].g = 1.0f ;
						Mesh->VertexColors[ Mesh->VertexColorNum ].b = 1.0f ;
						Mesh->VertexColors[ Mesh->VertexColorNum ].a = 1.0f ;

						// �ʂ̃J���[���̓f�t�H���g�ł͐^�����ɂ���
						// ���łɑS�C���f�b�N�X�̐����擾����
						TotalIndexNum = 0 ;
						for( j = 0 ; j < ( int )Mesh->FaceNum ; j ++ )
						{
							TotalIndexNum += ( int )Mesh->Faces[ j ].IndexNum ;
							for( k = 0 ; k < ( int )Mesh->Faces[ j ].IndexNum ; k ++ )
							{
								Mesh->Faces[ j ].VertexColorIndex[ k ] = Mesh->VertexColorNum ;
							}
						}

						// ���b�V�����g�p���Ă��钸�_�̃}�b�v���쐬����
						{
							FaceUseVertexIndexBufferSize = ( int )( sizeof( int ) * ( Mesh->PositionNum * 2 + TotalIndexNum * 2 ) ) ;
							FaceUseVertexIndexBuffer     = ( int * )DXALLOC( ( size_t )FaceUseVertexIndexBufferSize ) ;
							if( FaceUseVertexIndexBuffer == NULL )
							{
								DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xc3\x30\xb7\x30\xe5\x30\x4c\x30\x7f\x4f\x28\x75\x57\x30\x66\x30\x44\x30\x8b\x30\x02\x98\xb9\x70\x6e\x30\xde\x30\xc3\x30\xd7\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���b�V�����g�p���Ă��钸�_�̃}�b�v���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
								return -1 ;
							}
							_MEMSET( FaceUseVertexIndexBuffer, 0, ( size_t )FaceUseVertexIndexBufferSize ) ;
							FaceUseVertexIndexList = FaceUseVertexIndexBuffer + Mesh->PositionNum * 2 ;

							// �܂��͊e���_���g�p����Ă��鐔���擾
							for( j = 0 ; j < ( int )Mesh->FaceNum ; j ++ )
							{
								for( l = 0 ; l < ( int )Mesh->Faces[ j ].IndexNum ; l ++ )
								{
									pFUVIB = &FaceUseVertexIndexBuffer[ Mesh->Faces[ j ].VertexIndex[ l ] * 2 ] ;
									pFUVIB[ 0 ] ++ ;
								}
							}

							// �e���_���g�p���Ă���ʂ̏�񂪊i�[����Ă���o�b�t�@�ʒu���Z�b�g
							TotalIndexNum = 0 ;
							pFUVIB = FaceUseVertexIndexBuffer ;
							for( j = 0 ; j < ( int )Mesh->PositionNum ; j ++, pFUVIB += 2 )
							{
								pFUVIB[ 1 ] = TotalIndexNum ;
								TotalIndexNum += pFUVIB[ 0 ] ;

								// ���ɃJ�E���^�Ƃ��Ďg�p����̂Ŗʂ̐��̓��Z�b�g����
								pFUVIB[ 0 ] = 0 ;
							}

							// �e���_���g�p���Ă���ʂ̏����Z�b�g
							for( j = 0 ; j < ( int )Mesh->FaceNum ; j ++ )
							{
								for( l = 0 ; l < ( int )Mesh->Faces[ j ].IndexNum ; l ++ )
								{
									pFUVIB = &FaceUseVertexIndexBuffer[ Mesh->Faces[ j ].VertexIndex[ l ] * 2 ] ;
									pFUVIL = &FaceUseVertexIndexList[ pFUVIB[ 1 ] * 2 ] ;

									pFUVIL[ pFUVIB[ 0 ] * 2 + 0 ] = j ;
									pFUVIL[ pFUVIB[ 0 ] * 2 + 1 ] = l ;
									pFUVIB[ 0 ] ++ ;
								}
							}
						}

						// ���_�J���[�f�[�^�̃Z�b�g
						IC = MS->VertexColor->vertexColors ;
						for( j = 0 ; j < ( int )MS->VertexColor->nVertexColors ; j ++, IC ++ )
						{
							Mesh->VertexColors[ j ].r = IC->indexColor.red   / 255.0f ;
							Mesh->VertexColors[ j ].g = IC->indexColor.green / 255.0f ;
							Mesh->VertexColors[ j ].b = IC->indexColor.blue  / 255.0f ;
							Mesh->VertexColors[ j ].a = IC->indexColor.alpha / 255.0f ;

							// �w��ԍ��̒��_���W���g�p���Ă���ʂ̒��_�J���[�� j �Ԗڂ̒��_�J���[�ɂ���
							pFUVIB = &FaceUseVertexIndexBuffer[ IC->index * 2 ] ;
							pFUVIL = &FaceUseVertexIndexList[ pFUVIB[ 1 ] * 2 ] ;
							for( k = 0 ; k < pFUVIB[ 0 ] ; k ++ )
							{
								Mesh->Faces[ pFUVIL[ 0 ] ].VertexColorIndex[ pFUVIL[ 1 ] ] = ( DWORD )j ;
								pFUVIL += 2 ;
							}
						}

						// ���b�V�����g�p���Ă��钸�_�̃}�b�v�p�Ɋm�ۂ��Ă��������������
						DXFREE( FaceUseVertexIndexBuffer ) ;
						FaceUseVertexIndexBuffer = NULL ;
					}
				}
				break ;

			case TEMP_ANIMATIONSET :	// �A�j���[�V�����Z�b�g
				X_ANIMATIONSET *AS ;
				AS = ( X_ANIMATIONSET * )DOFF( O ) ;

				// �A�j���[�V�����Z�b�g�̒ǉ�
				AnimSet = MV1RAddAnimSet( RModel, O->Name ) ;
				if( AnimSet == NULL )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xbb\x30\xc3\x30\xc8\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �A�j���[�V�����Z�b�g�I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
					return -1 ;
				}

				// �p�����[�^�̃Z�b�g
				AnimSet->StartTime = 0.0f ;
				AnimSet->EndTime = ( float )AS->MaxCount ;

				// �A�j���[�V�������̃Z�b�g
				for( i = 0 ; i < AS->AnimationNum ; i ++ )
				{
					X_ANIMATION *AN = AS->Animation[ i ] ;

					// �Q�ƂɎ��s���Ă����牽�����Ȃ�
					if( AN->Frame == NULL ) continue ;

					// �A�j���[�V�����̒ǉ�
					Anim = MV1RAddAnim( RModel, AnimSet ) ;
					if( Anim == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �A�j���[�V�����I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
						return -1 ;
					}

					// �p�����[�^�Z�b�g
					Anim->TargetFrameIndex = AN->Frame->MV1Frame->Index ;
					Anim->MaxTime = ( float )AN->MaxCount ;

					// �A�j���[�V�����L�[�̐������J��Ԃ�
					for( j = 0 ; j < ( int )AN->AnimationKeysNum ; j ++ )
					{
						X_ANIMATIONKEY *AK = AN->AnimationKeys[ j ] ;
						X_TIMEDFLOATKEYS *TimeKeys ;
						int AllocSize = 0 ;

						// �A�j���[�V�����L�[�̒ǉ�
						AnimKeySet = MV1RAddAnimKeySet( RModel, Anim ) ;
						if( AnimKeySet == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xbb\x30\xc3\x30\xc8\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �A�j���[�V�����L�[�Z�b�g�I�u�W�F�N�g�̒ǉ��Ɏ��s���܂���\n" @*/ )) ;
							return -1 ;
						}

						// �p�����[�^�̃Z�b�g
						AnimKeySet->TotalTime = ( float )AK->TotalCount ;
						AnimKeySet->Num       = ( int )AK->nKeys ;

						// �L�[�^�C�v�̃Z�b�g
						switch( AK->keyType )
						{
						case 0 :
							AnimKeySet->DataType = MV1_ANIMKEY_DATATYPE_ROTATE ;
							AnimKeySet->Type = MV1_ANIMKEY_TYPE_QUATERNION_X ;
							break ;

						case 1 :
							AnimKeySet->DataType = MV1_ANIMKEY_DATATYPE_SCALE ;
							AnimKeySet->Type = MV1_ANIMKEY_TYPE_VECTOR ;
							break ;

						case 2 :
							AnimKeySet->DataType = MV1_ANIMKEY_DATATYPE_TRANSLATE ;
							AnimKeySet->Type = MV1_ANIMKEY_TYPE_VECTOR ;
							break ;

						case 3 :
						case 4 :
							AnimKeySet->DataType = MV1_ANIMKEY_DATATYPE_MATRIX4X4C ;
							AnimKeySet->Type = MV1_ANIMKEY_TYPE_MATRIX4X4C ;
							break ;
						}

						// �L�[���i�[���郁�����̈�̊m��
						switch( AnimKeySet->Type )
						{
						case MV1_ANIMKEY_TYPE_QUATERNION_X : AllocSize = sizeof( FLOAT4                  ) ;	break ;
						case MV1_ANIMKEY_TYPE_VECTOR :       AllocSize = sizeof( VECTOR                  ) ;	break ;
						case MV1_ANIMKEY_TYPE_MATRIX4X4C :   AllocSize = sizeof( MV1_ANIM_KEY_MATRIX4X4C ) ;	break ;
						}
						AnimKeySet->KeyFloat4 = ( FLOAT4 * )ADDMEMAREA( ( AllocSize + sizeof( float ) ) * AnimKeySet->Num, &RModel->Mem ) ;
						if( AnimKeySet->KeyFloat4 == NULL )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �A�j���[�V�����L�[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
							return -1 ;
						}
						AnimKeySet->KeyTime = ( float * )( ( BYTE * )AnimKeySet->KeyFloat4 + AllocSize * AnimKeySet->Num ) ;

						// �L�[�f�[�^�̃T�C�Y�����Z����
						RModel->AnimKeyDataSize += ( AllocSize + sizeof( float ) ) * AnimKeySet->Num ;

						// �L�[�̃R�s�[
						TimeKeys = AK->keys ;
						switch( AnimKeySet->Type )
						{
						case MV1_ANIMKEY_TYPE_QUATERNION_X :		// ��]�^�C�v�̏ꍇ
							for( k = 0 ; k < AnimKeySet->Num ; k ++, TimeKeys ++ )
							{
								AnimKeySet->KeyTime[ k ]       = ( float )TimeKeys->time ;
								AnimKeySet->KeyFloat4[ k ].x = TimeKeys->tfkeys.values[ 1 ] ;
								AnimKeySet->KeyFloat4[ k ].y = TimeKeys->tfkeys.values[ 2 ] ;
								AnimKeySet->KeyFloat4[ k ].z = TimeKeys->tfkeys.values[ 3 ] ;
								AnimKeySet->KeyFloat4[ k ].w = TimeKeys->tfkeys.values[ 0 ] ;
							}
							break ;

						case MV1_ANIMKEY_TYPE_VECTOR :		// �x�N�^�[�^�C�v�̏ꍇ
							for( k = 0 ; k < AnimKeySet->Num ; k ++, TimeKeys ++ )
							{
								AnimKeySet->KeyTime[ k ]    = ( float )TimeKeys->time ;
								AnimKeySet->KeyVector[ k ].x = TimeKeys->tfkeys.values[ 0 ] ;
								AnimKeySet->KeyVector[ k ].y = TimeKeys->tfkeys.values[ 1 ] ;
								AnimKeySet->KeyVector[ k ].z = TimeKeys->tfkeys.values[ 2 ] ;
							}
							break ;

						case MV1_ANIMKEY_TYPE_MATRIX4X4C :		// �s��^�C�v�̏ꍇ
//							AnimSet->IsMatrixLinearBlend = 1 ;
							for( k = 0 ; k < AnimKeySet->Num ; k ++, TimeKeys ++ )
							{
								AnimKeySet->KeyTime[ k ]                        = ( float )TimeKeys->time ;
								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 0 ][ 0 ] = TimeKeys->tfkeys.values[ 0 ] ;
								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 0 ][ 1 ] = TimeKeys->tfkeys.values[ 1 ] ;
								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 0 ][ 2 ] = TimeKeys->tfkeys.values[ 2 ] ;

								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 1 ][ 0 ] = TimeKeys->tfkeys.values[ 4 ] ;
								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 1 ][ 1 ] = TimeKeys->tfkeys.values[ 5 ] ;
								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 1 ][ 2 ] = TimeKeys->tfkeys.values[ 6 ] ;

								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 2 ][ 0 ] = TimeKeys->tfkeys.values[ 8 ] ;
								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 2 ][ 1 ] = TimeKeys->tfkeys.values[ 9 ] ;
								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 2 ][ 2 ] = TimeKeys->tfkeys.values[ 10 ] ;

								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 3 ][ 0 ] = TimeKeys->tfkeys.values[ 12 ] ;
								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 3 ][ 1 ] = TimeKeys->tfkeys.values[ 13 ] ;
								AnimKeySet->KeyMatrix4x4C[ k ].Matrix[ 3 ][ 2 ] = TimeKeys->tfkeys.values[ 14 ] ;
							}
							break ;
						}
					}
				}
				break ;
			}

			O = O->NextData ;
		}
	}

	// �I��
	return 0 ;
}




// �I�u�W�F�N�g���X�^�b�N�ɉ�����
static int IncStackObject( X_STACK *Stack, X_OBJECT *Object )
{
	// ���Ɍ��E�������ꍇ�̓G���[
	if( Stack->StackNum == MAX_STACKNUM )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x8e\x96\x64\x5c\xcb\x69\x20\x90\x6e\x30\x50\x96\x4c\x75\x70\x65\x20\x00\x25\x00\x64\x00\x20\x00\x92\x30\x85\x8d\x48\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�K�w�\���̌��E�� %d �𒴂��܂���" @*/, MAX_STACKNUM )) ;
		return -1 ;
	}

	// �X�^�b�N�ɃI�u�W�F�N�g��������
	Stack->Stack[Stack->StackNum] = Object ;
	Stack->StackNum ++ ;

	// �I��
	return 0 ;
}

// �I�u�W�F�N�g�̃X�^�b�N������炷
static int DecStackObject( X_STACK *Stack )
{
	// �X�^�b�N�̐����ւ炷
	Stack->StackNum -- ;

	// �I��
	return 0 ;
}






















// �e���v���[�g��͗p�֐�

// ������e���v���[�g�̉��
static int TempString( X_PSTRING *PStr, X_MODEL *Model, X_STRING *String )
{
	X_STRING *S = String ;
	char StrB[256] ;

	if( !PStr->binf )
	{
		// �_�u���R�[�e�[�V����������Ƃ���܂ŃX�L�b�v
		PStrMoveP( PStr, "\"" ) ;

		// ���̃_�u���R�[�e�[�V����������Ƃ���܂ł��擾
		SetPStr( PStr, NULL, 0, "\"" ) ;
		PStrGet( PStr, StrB, sizeof( StrB ) ) ;
		SetPStr( PStr, NULL, 0, DefSkipStr ) ;
		PStr->StrOffset ++ ;
	}
	else
	{
		PStrMoveB( PStr, TOKEN_STRING ) ;
		if( PStr->StrOffset >= PStr->StrSize ) return -1 ;
		_STRNCPY_S( StrB, sizeof( StrB ), PSTRP(PStr,6), ( int )( PSTRDWORD(PStr,2) + 1 ) ) ;
		StrB[PSTRDWORD(PStr,2)] = '\0' ;

		PStrMoveOneB(PStr) ;
	}

	// ��������i�[���郁�����̈���m��
	if( ( S->String = ( char * )ADDMEMAREA( _STRLEN( StrB ) + 1, &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDA( ( "Load XFile : \x95\xb6\x8e\x9a\x97\xf1 %s \x82\xf0\x8a\x69\x94\x5b\x82\xb7\x82\xe9\x83\x81\x83\x82\x83\x8a\x97\xcc\x88\xe6\x82\xcc\x8a\x6d\x95\xdb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Load XFile : ������ %s ���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/, StrB ) ) ;
		return -1 ;
	}

	// ������̃R�s�[
	_STRCPY( S->String, StrB ) ;

	// �I��
	return 0 ;
}

// ���[�h�e���v���[�g�̉��
static int TempWord( X_PSTRING *PStr, X_MODEL * /*Model*/, X_WORD *Word )
{
	char StrB[80] ;

	// ���̕������ǂ�
	PStrGet( PStr, StrB, sizeof( StrB ) ) ;

	// ���l�Ɋ�����
	Word->w = ( WORD )_ATOI( StrB ) ;

	// �I��
	return 0 ;
}

// �_�u�����[�h�e���v���[�g�̉��
static int TempDword( X_PSTRING *PStr, X_MODEL * /*Model*/, X_DWORD *Dword )
{
	char StrB[80] ;

	if( !PStr->binf )
	{
		// ���̕������ǂ�
		PStrGet( PStr, StrB, sizeof( StrB ) ) ;

		// ���l�Ɋ�����
		Dword->d = ( DWORD )_ATOI( StrB ) ;
	}
	else
	{
		switch( PSTRWORD(PStr,0) )
		{
		case TOKEN_INTEGER :
			Dword->d = PSTRDWORD(PStr,2) ;
			break ;

		case TOKEN_INTEGER_LIST :
			if( PSTRDWORD(PStr,2) != 1 ) return -1 ;
			Dword->d = PSTRDWORD(PStr,6) ;
			break ;
		}
		PStrMoveOneB( PStr ) ;
	}

	// �I��
	return 0 ;
}

// �t���[�g�e���v���[�g�̉��
static int TempFloat( X_PSTRING *PStr, X_MODEL * /*Model*/, X_FLOAT *Float )
{
	char StrB[80] ;

	// ���̕������ǂ�
	PStrGet( PStr, StrB, sizeof( StrB ) ) ;

	// ���l�Ɋ�����
	Float->f = ( float )_ATOF( StrB ) ;

	// �I��
	return 0 ;
}

// �x�N�g���e���v���[�g�̉��
static int TempVector( X_PSTRING *PStr, X_MODEL *Model, X_VECTOR *Vector )
{
	// �t���[�g�^���R�ǂݍ���
	TempFloat( PStr, Model, ( X_FLOAT * )&Vector->dat.x ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Vector->dat.y ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Vector->dat.z ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	// �I��
	return 0 ;
}

// �e�N�X�`�����W�Q�����e���v���[�g�̉��
static int TempCoords2d( X_PSTRING *PStr, X_MODEL *Model, X_COORDS2D *Coords2d )
{
	// �t���[�g�^���Q�ǂݍ���
	TempFloat( PStr, Model, ( X_FLOAT * )&Coords2d->u ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Coords2d->v ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	// �I��
	return 0 ;
}

// �S���S�s��e���v���[�g�̉��
static int TempMatrix4x4( X_PSTRING *PStr, X_MODEL *Model, X_MATRIX4X4 *Matrix )
{
	// �t���[�g�^���P�U���ǂݍ���
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[0][0] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[0][1] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[0][2] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[0][3] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;

	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[1][0] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[1][1] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[1][2] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[1][3] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;

	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[2][0] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[2][1] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[2][2] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[2][3] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;

	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[3][0] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[3][1] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[3][2] ) ;	if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&Matrix->matrix[3][3] ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	// �I��
	return 0 ;
}

// �A���t�@���J���[�e���v���[�g�̉��
static int TempColorRGBA( X_PSTRING *PStr, X_MODEL *Model, X_COLORRGBA *ColorRGBA )
{
	// �t���[�g�^���S�ǂݍ���
	TempFloat( PStr, Model, ( X_FLOAT * )&ColorRGBA->red ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&ColorRGBA->green ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&ColorRGBA->blue ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&ColorRGBA->alpha ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	// �I��
	return 0 ;
}

// �J���[�e���v���[�g�̉��
static int TempColorRGB( X_PSTRING *PStr, X_MODEL *Model, X_COLORRGB *ColorRGB )
{
	// �t���[�g�^���S�ǂݍ���
	TempFloat( PStr, Model, ( X_FLOAT * )&ColorRGB->red ) ;		if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&ColorRGB->green ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	TempFloat( PStr, Model, ( X_FLOAT * )&ColorRGB->blue ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	// �I��
	return 0 ;
}

// �}�e���A���e���v���[�g�̉��
static int TempMaterial( X_PSTRING *PStr, X_MODEL *Model, X_MATERIAL *Material )
{
	if( !PStr->binf )
	{
		// �t�F�C�X�J���[��ǂݍ���
		TempColorRGBA( PStr, Model, &Material->faceColor ) ;		if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

		// �p���[��ǂݍ���
		TempFloat( PStr, Model, ( X_FLOAT * )&Material->power ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

		// �X�y�L�����J���[�ƃG�~�b�V�u�J���[��ǂݍ���
		TempColorRGB( PStr, Model, &Material->specularColor ) ;		if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
		TempColorRGB( PStr, Model, &Material->emissiveColor ) ;		if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	}
	else
	{
		if( PSTRWORD(PStr,0) != TOKEN_FLOAT_LIST )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : Material FloadList\n" )) ;
			return -1 ;
		}
		if( PSTRDWORD(PStr,2) != 11 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : Material Not 11\n" )) ;
			return -1 ;
		}

		if( PStr->f64f )
		{
			// �t�F�C�X�E�p���[�E�X�P�L�����E�G�~�b�V�u��ǂݍ���
			Material->faceColor.red   = PSTRDOUBLE(PStr,6) ;
			Material->faceColor.green = PSTRDOUBLE(PStr,14) ;
			Material->faceColor.blue  = PSTRDOUBLE(PStr,22) ;
			Material->faceColor.alpha = PSTRDOUBLE(PStr,30) ;
			Material->power = PSTRDOUBLE(PStr,38) ;
			Material->specularColor.red   = PSTRDOUBLE(PStr,46) ;
			Material->specularColor.green = PSTRDOUBLE(PStr,54) ;
			Material->specularColor.blue  = PSTRDOUBLE(PStr,62) ;
			Material->emissiveColor.red   = PSTRDOUBLE(PStr,70) ;
			Material->emissiveColor.green = PSTRDOUBLE(PStr,78) ;
			Material->emissiveColor.blue  = PSTRDOUBLE(PStr,86) ;
		}
		else
		{
			// �t�F�C�X�E�p���[�E�X�P�L�����E�G�~�b�V�u��ǂݍ���
			Material->faceColor.red = PSTRFLOAT(PStr,6) ;	Material->faceColor.green = PSTRFLOAT(PStr,10) ;	Material->faceColor.blue = PSTRFLOAT(PStr,14) ;	Material->faceColor.alpha = PSTRFLOAT(PStr,18) ;
			Material->power = PSTRFLOAT(PStr,22) ;
			Material->specularColor.red = PSTRFLOAT(PStr,26) ;	Material->specularColor.green = PSTRFLOAT(PStr,30) ;	Material->specularColor.blue = PSTRFLOAT(PStr,34) ;
			Material->emissiveColor.red = PSTRFLOAT(PStr,38) ;	Material->emissiveColor.green = PSTRFLOAT(PStr,42) ;	Material->emissiveColor.blue = PSTRFLOAT(PStr,46) ;
		}

		PStrMoveOneB( PStr ) ;
	}

	// �I��
	return 0 ;
}

// �e�N�X�`���t�@�C���l�[���e���v���[�g�̉��
static int TempTextureFilename( X_PSTRING *PStr, X_MODEL *Model, X_TEXTUREFILENAME *TexFileName )
{
	int i ;
	X_TEXTUREFILENAME_LIST *Name, *BackName ;

	// �e�N�X�`���t�@�C���l�[����ǂݍ���
	TempString( PStr, Model, &TexFileName->filename ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	// ���܂œ������O�̃e�N�X�`���t�@�C�����������������ׂ�
	Name = Model->FirstTextureFileName ;
	BackName = Name ;
	for( i = 0 ; i < Model->TextureNum && _STRCMP( Name->FileName->filename.String, TexFileName->filename.String ) != 0 ; i ++, BackName = Name, Name = Name->Next ){}

	// ����������ǉ�
	if( i == Model->TextureNum )
	{
		// �V���ȃI�u�W�F�N�g�̃��������m��
		Name = ( X_TEXTUREFILENAME_LIST * )ADDMEMAREA( sizeof( X_TEXTUREFILENAME_LIST ), &Model->XModelMem ) ; 
		if( Name == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xcd\x30\xfc\x30\xe0\x30\xea\x30\xb9\x30\xc8\x30\xaa\x30\xd6\x30\xb8\x30\xa7\x30\xaf\x30\xc8\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �e�N�X�`���t�@�C���l�[�����X�g�I�u�W�F�N�g���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			return -1 ;
		}

		// �p�����[�^�̃Z�b�g
		Name->FileName = TexFileName ;
		Name->Next = NULL ;

		// ���X�g�ւ̒ǉ�
		if( Model->TextureNum == 0 )
		{
			Model->FirstTextureFileName = Name ;
		}
		else
		{
			BackName->Next = Name ;
		}

		// �����C���N�������g
		Model->TextureNum ++ ;
	}

	// �C���f�b�N�X���Z�b�g
	TexFileName->Index = i ;

	// �I��
	return 0 ;
}

// ���b�V���t�F�C�X�e���v���[�g�̉��
static int TempMeshFace( X_PSTRING *PStr, X_MODEL *Model, X_MESHFACE *MeshFace )
{
	DWORD i ;

	if( !PStr->binf )
	{
		// �t�F�C�X�̐���ǂݍ���
		TempDword( PStr, Model, ( X_DWORD * )&MeshFace->nFaceVertexIndices ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
		if( MeshFace->nFaceVertexIndices > MAX_INDEXNUM )
		{
			MeshFace->faceVertexIndices = ( DWORD * )ADDMEMAREA( sizeof( DWORD ) * MeshFace->nFaceVertexIndices, &Model->XModelMem ) ;
			if( MeshFace->faceVertexIndices == NULL )
			{
	//			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : ��̖ʂŎg�p�ł��钸�_���̍ő吔 %d �𒴂����ʂ�����܂���( %d �̒��_ )", MAX_INDEXNUM, MeshFace->nFaceVertexIndices ) ) ;
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xd5\x30\xa7\x30\xa4\x30\xb9\x30\x92\x30\xcb\x69\x10\x62\x59\x30\x8b\x30\x02\x98\xb9\x70\x6e\x30\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x20\x00\x25\x00\x64\x00\x20\x00\x0b\x50\x6e\x30\x02\x98\xb9\x70\x20\x00\x29\x00\x00"/*@ L"Load XFile : �t�F�C�X���\�����钸�_�̃C���f�b�N�X��ۑ����郁�����̈�̊m�ۂɎ��s���܂���( %d �̒��_ )" @*/, MeshFace->nFaceVertexIndices ) ) ;
				return -1 ;
			}
		}

		// �t�F�C�X�̐������C���f�b�N�X��ǂݍ���
		if( MeshFace->nFaceVertexIndices != 0 )
		{
			if( MeshFace->faceVertexIndices != NULL )
			{
				for( i = 0 ; i < MeshFace->nFaceVertexIndices - 1 ; i ++ )
				{
					TempDword( PStr, Model, ( X_DWORD * )&MeshFace->faceVertexIndices[i] ) ;
					PStrMoveP( PStr, ";," ) ;
				}
				TempDword( PStr, Model, ( X_DWORD * )&MeshFace->faceVertexIndices[i] ) ;
			}
			else
			{
				for( i = 0 ; i < MeshFace->nFaceVertexIndices - 1 ; i ++ )
				{
					TempDword( PStr, Model, ( X_DWORD * )&MeshFace->faceVertexIndicesFixedLength[i] ) ;
					PStrMoveP( PStr, ";," ) ;
				}
				TempDword( PStr, Model, ( X_DWORD * )&MeshFace->faceVertexIndicesFixedLength[i] ) ;
			}
			PStrMoveP( PStr, ";," ) ;
		}
	}
	else
	{
		DWORD j ;

		// �t�F�C�X�̐���ǂݍ���
		MeshFace->nFaceVertexIndices = PSTRDWORD(PStr,6) ;
		if( MeshFace->nFaceVertexIndices > MAX_INDEXNUM )
		{
			MeshFace->faceVertexIndices = ( DWORD * )ADDMEMAREA( sizeof( DWORD ) * MeshFace->nFaceVertexIndices, &Model->XModelMem ) ;
			if( MeshFace->faceVertexIndices == NULL )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xd5\x30\xa7\x30\xa4\x30\xb9\x30\x92\x30\xcb\x69\x10\x62\x59\x30\x8b\x30\x02\x98\xb9\x70\x6e\x30\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x20\x00\x25\x00\x64\x00\x20\x00\x0b\x50\x6e\x30\x02\x98\xb9\x70\x20\x00\x29\x00\x00"/*@ L"Load XFile : �t�F�C�X���\�����钸�_�̃C���f�b�N�X��ۑ����郁�����̈�̊m�ۂɎ��s���܂���( %d �̒��_ )" @*/, MeshFace->nFaceVertexIndices ) ) ;
				return -1 ;
			}
		}

		// �t�F�C�X�̐������C���f�b�N�X��ǂݍ���
		j = 10 ;
		if( MeshFace->faceVertexIndices != NULL )
		{
			for( i = 0 ; i < MeshFace->nFaceVertexIndices ; i ++, j += 4 )
			{
				MeshFace->faceVertexIndices[i] = PSTRDWORD(PStr,j) ;
			}
		}
		else
		{
			for( i = 0 ; i < MeshFace->nFaceVertexIndices ; i ++, j += 4 )
			{
				MeshFace->faceVertexIndicesFixedLength[i] = PSTRDWORD(PStr,j) ;
			}
		}

		PStrMoveOneB(PStr) ;
	}


	// �I��
	return 0 ;
}

// ���b�V���e�N�X�`���[�R�[�h�X�e���v���[�g�̉��
static int TempMeshTextureCoords( X_PSTRING *PStr, X_MODEL *Model, X_MESHTEXTURECOORDS *MeshTextureCoords )
{
	DWORD i, j ;
	X_COORDS2D *C ;

	// �e�N�X�`�����W�̐��𐔂���
	TempDword( PStr, Model, ( X_DWORD * )&MeshTextureCoords->nTextureCoords ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	// �e�N�X�`�����W���i�[���郁�����̊m��
	if( ( MeshTextureCoords->textureCoords = ( X_COORDS2D * )ADDMEMAREA( sizeof( X_COORDS2D ) * MeshTextureCoords->nTextureCoords, &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x55\x00\x56\x00\xa7\x5e\x19\x6a\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : UV���W���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// �e�N�X�`�����W�̐������C���f�b�N�X��ǂݍ���
	if( !PStr->binf )
	{
		if( MeshTextureCoords->nTextureCoords != 0 )
		{
			C = MeshTextureCoords->textureCoords ;
			for( i = 0 ; i < MeshTextureCoords->nTextureCoords - 1 ; i ++, C ++ )
			{
				TempCoords2d( PStr, Model, C ) ;

				// �ςȋL�q�p�̃v���O����
				PStrMoveP( PStr, "0123456789.-+," ) ;
				if( ( PStr->StrBuf[ PStr->StrOffset - 1 ] >= '0' && PStr->StrBuf[ PStr->StrOffset - 1 ] <= '9' ) ||
					PStr->StrBuf[ PStr->StrOffset - 1 ] == '.' ||
					PStr->StrBuf[ PStr->StrOffset - 1 ] == '+' ||
					PStr->StrBuf[ PStr->StrOffset - 1 ] == '-' )
				{
					PStr->StrOffset -- ;
				}
			}
			TempCoords2d( PStr, Model, C ) ;

			// �ςȋL�q�p�� } ���܂߂�
			PStrMoveP( PStr, "};," ) ;

			// �ςȋL�q�p�̃v���O����
			if( PStr->StrBuf[ PStr->StrOffset - 1 ] == '}' )
			{
				PStr->StrOffset -- ;
			}
		}
	}
	else
	{
		if( PSTRWORD(PStr, 0) != TOKEN_FLOAT_LIST )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : MeshTextureCorrds FloadList\n" )) ;
			return -1 ;
		}
		if( PSTRDWORD(PStr, 2) != MeshTextureCoords->nTextureCoords * 2 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : MeshTextureCorrds Not %d ( %d )\n", MeshTextureCoords->nTextureCoords * 2, PSTRDWORD(PStr, 2) ) ) ;
			return -1 ;
		}

		j = 6 ;
		C = MeshTextureCoords->textureCoords ;
		if( PStr->f64f )
		{
			for( i = 0 ; i < MeshTextureCoords->nTextureCoords ; i ++, C ++, j += 16 )
			{
				C->u = PSTRDOUBLE(PStr,j) ;  C->v = PSTRDOUBLE(PStr,j+8) ;
			}
		}
		else
		{
			for( i = 0 ; i < MeshTextureCoords->nTextureCoords ; i ++, C ++, j += 8 )
			{
				C->u = PSTRFLOAT(PStr,j) ;  C->v = PSTRFLOAT(PStr,j+4) ;
			}
		}

		PStrMoveOneB( PStr ) ;
	}

	// �I��
	return 0 ;
}

// ���b�V���m�[�}���X�e���v���[�g�̉��
static int TempMeshNormals( X_PSTRING *PStr, X_MODEL *Model, X_MESHNORMALS *MeshNormals )
{
	DWORD i, j, k ;
	X_VECTOR *V ;
	X_MESHFACE *MF ;

	// �@���̐����擾����
	TempDword( PStr, Model, ( X_DWORD * )&MeshNormals->nNormals ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	// �@�����i�[���郁�����̊m��
	if( ( MeshNormals->normals = ( X_VECTOR * )ADDMEMAREA( sizeof( X_VECTOR ) * MeshNormals->nNormals, &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xd5\x6c\xda\x7d\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �@�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// �@���̐������C���f�b�N�X��ǂݍ���
	if( !PStr->binf )
	{
		if( MeshNormals->nNormals != 0 )
		{
			V = MeshNormals->normals ;
			for( i = 0 ; i < MeshNormals->nNormals - 1 ; i ++, V ++ )
			{
				TempVector( PStr, Model, V ) ;
				if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
			}
			TempVector( PStr, Model, V ) ;
			if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
		}
	}
	else
	{
		if( PSTRWORD(PStr, 0) != TOKEN_FLOAT_LIST )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : MeshNormals FloadList\n" )) ;
			return -1 ;
		}
		if( PSTRDWORD(PStr, 2) != MeshNormals->nNormals * 3 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : MeshNormals Not %d ( %d )\n", MeshNormals->nNormals * 3, PSTRDWORD(PStr, 2) ) ) ;
			return -1 ;
		}

		V = MeshNormals->normals ;
		j = 6 ;
		if( PStr->f64f )
		{
			for( i = 0 ; i < MeshNormals->nNormals ; i ++, V ++, j += 24 )
			{
				V->dat.x = (float)PSTRDOUBLE(PStr,j) ; V->dat.y = (float)PSTRDOUBLE(PStr,j+8) ; V->dat.z = (float)PSTRDOUBLE(PStr,j+16) ;
			}
		}
		else
		{
			for( i = 0 ; i < MeshNormals->nNormals  ; i ++, V ++, j += 12 )
			{
				V->dat.x = PSTRFLOAT(PStr,j) ; V->dat.y = PSTRFLOAT(PStr,j+4) ; V->dat.z = PSTRFLOAT(PStr,j+8) ;
			}
		}
		PStrMoveOneB( PStr ) ;
	}

	// �@���t�F�C�X�̐����擾����
//	TempDword( PStr, Model, ( X_DWORD * )&MeshNormals->nFaceNormals ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";" ) ;
	if( !PStr->binf )
	{
		TempDword( PStr, Model, ( X_DWORD * )&MeshNormals->nFaceNormals ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	}
	else
	{
		switch( PSTRWORD(PStr,0) )
		{
		case TOKEN_INTEGER :		MeshNormals->nFaceNormals = PSTRDWORD(PStr,2) ; break ;
		case TOKEN_INTEGER_LIST :	MeshNormals->nFaceNormals = PSTRDWORD(PStr,6) ; break ;
		}
	}

	// �@���t�F�C�X���i�[���郁�����̊m��
	if( ( MeshNormals->faceNormals = ( X_MESHFACE * )ADDMEMAREA( sizeof( X_MESHFACE ) * MeshNormals->nFaceNormals, &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xd5\x6c\xda\x7d\x62\x97\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �@���ʏ����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// �@���t�F�C�X�̐������t�F�C�X��ǂݍ���
	if( !PStr->binf )
	{
		if( MeshNormals->nFaceNormals != 0 )
		{
			MF = MeshNormals->faceNormals ;
			for( i = 0 ; i < MeshNormals->nFaceNormals - 1 ; i ++, MF ++ )
			{
				if( !PStr->binf ) PStrMoveNum( PStr );
				if( TempMeshFace( PStr, Model, MF ) < 0 )
				{
					return -1 ;
				}
			}
			if( !PStr->binf ) PStrMoveNum( PStr );
			if( TempMeshFace( PStr, Model, MF ) < 0 )
			{
				return -1 ;
			}
		}
	}
	else
	{
//		if( PSTRWORD(PStr, 0) != TOKEN_INTEGER_LIST ) return DXST_ERRORLOG_ADDW( L"Error aadfa;bb" ) ;

		j = 0 ;
		switch( PSTRWORD(PStr,0) )
		{
		case TOKEN_INTEGER : PStrMoveOneB( PStr ) ; j = 6 ; break ;
		case TOKEN_INTEGER_LIST : j = 10 ; break ;
		}

		MF = MeshNormals->faceNormals ;
		for( i = 0 ; i < MeshNormals->nFaceNormals ; i ++, MF ++ )
		{
			MF->nFaceVertexIndices = PSTRDWORD(PStr,j) ; j += 4 ;
			if( MF->faceVertexIndices != NULL )
			{
				for( k = 0 ; k < MF->nFaceVertexIndices ; k ++, j += 4 )
				{
					MF->faceVertexIndices[k] = PSTRDWORD(PStr,j) ;
				}
			}
			else
			{
				for( k = 0 ; k < MF->nFaceVertexIndices ; k ++, j += 4 )
				{
					MF->faceVertexIndicesFixedLength[k] = PSTRDWORD(PStr,j) ;
				}
			}
		}
		PStrMoveOneB( PStr ) ;
	}

	// �I��
	return 0 ;
}


// ���b�V���}�e���A�����X�g�e���v���[�g�̉��
static int TempMeshMaterialList( X_PSTRING *PStr, X_MODEL *Model, X_MESHMATERIALLIST *MeshMaterialList )
{
	DWORD i, MaxIndex ;
	DWORD *D ;

	MaxIndex = 0 ;
	if( !PStr->binf )
	{
		// �}�e���A���̎�ނ̐����擾����
		TempDword( PStr, Model, ( X_DWORD * )&MeshMaterialList->nMaterials ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

		// �K���ʏ��̐����擾����
		TempDword( PStr, Model, ( X_DWORD * )&MeshMaterialList->nFaceIndexes ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	}
	else
	{
		if( PSTRWORD(PStr,0) != TOKEN_INTEGER_LIST )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : MeshMaterialList IntergerList\n" )) ;
			return -1 ;
		}

		// �}�e���A���̎�ނ̐����擾����
		MeshMaterialList->nMaterials = PSTRDWORD(PStr,6) ;

		// �K���ʏ��̐����擾����
		MeshMaterialList->nFaceIndexes = PSTRDWORD(PStr,10) ;
	}

	// �K���ʏ����i�[���郁�����̊m��
	if( ( MeshMaterialList->FaceIndexes = ( DWORD * )ADDMEMAREA( sizeof( DWORD ) * MeshMaterialList->nFaceIndexes, &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\x68\x30\x62\x97\x6e\x30\xa2\x95\xc2\x4f\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �}�e���A���Ɩʂ̊֌W�����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// �K���ʏ��̐������C���f�b�N�X��ǂݍ���
	if( !PStr->binf )
	{
		if( MeshMaterialList->nFaceIndexes != 0 )
		{
			D = MeshMaterialList->FaceIndexes ;
			for( i = 0 ; i < MeshMaterialList->nFaceIndexes - 1 ; i ++, D ++ )
			{
				TempDword( PStr, Model, ( X_DWORD * )D ) ;
				if( *D > MaxIndex ) MaxIndex = *D ;
				if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
			}
			TempDword( PStr, Model, ( X_DWORD * )D ) ;
			if( *D > MaxIndex ) MaxIndex = *D ;
			if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
		}
	}
	else
	{
		DWORD j ;

		j = 14 ;
		D = MeshMaterialList->FaceIndexes ;
		for( i = 0 ; i < MeshMaterialList->nFaceIndexes ; i ++, D ++, j += 4 )
		{
			*D = PSTRDWORD(PStr,j) ;
			if( *D > MaxIndex ) MaxIndex = *D ;
		}

		PStrMoveOneB(PStr) ;
	}

	if( MaxIndex + 1 > MeshMaterialList->nMaterials )
	{
//		DXST_ERRORLOG_ADDW( L"Load XFile : nMaterials �Ǝ��ۂɎg�p����Ă���}�e���A���̐����Ⴂ�܂�\n" ) ;
		DXST_ERRORLOG_ADDUTF16LE( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x6e\x00\x4d\x00\x61\x00\x74\x00\x65\x00\x72\x00\x69\x00\x61\x00\x6c\x00\x73\x00\x20\x00\x88\x30\x8a\x30\x27\x59\x4d\x30\x44\x30\xde\x30\xc6\x30\xea\x30\xa2\x30\xeb\x30\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : nMaterials ���傫���}�e���A���C���f�b�N�X������܂���\n" @*/ ) ;
		return -1 ;
	}

	// �I��
	return 0 ;
}

// �C���f�b�N�X�J���[�̉��
static int TempIndexedColor( X_PSTRING *PStr, X_MODEL *Model, X_INDEXEDCOLOR *IndexedColor )
{
	TempDword( PStr, Model, ( X_DWORD * )&IndexedColor->index ) ; 	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	if( !PStr->binf )
	{
		TempColorRGBA( PStr, Model, &IndexedColor->indexColor ) ;
		PStrMoveP( PStr, ";," ) ;
	}
	else
	{
		if( PSTRWORD(PStr, 0) != TOKEN_FLOAT_LIST )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : IndexedColor FloadList\n" )) ;
			return -1 ;
		}
		if( PStr->f64f )
		{
			IndexedColor->indexColor.red   = PSTRDOUBLE( PStr, 6 ) ;
			IndexedColor->indexColor.green = PSTRDOUBLE( PStr, 14 ) ;
			IndexedColor->indexColor.blue  = PSTRDOUBLE( PStr, 22 ) ;
			IndexedColor->indexColor.alpha = PSTRDOUBLE( PStr, 30 ) ;
		}
		else
		{
			IndexedColor->indexColor.red   = PSTRFLOAT( PStr, 6 ) ;
			IndexedColor->indexColor.green = PSTRFLOAT( PStr, 10 ) ;
			IndexedColor->indexColor.blue  = PSTRFLOAT( PStr, 14 ) ;
			IndexedColor->indexColor.alpha = PSTRFLOAT( PStr, 18 ) ;
		}

		PStrMoveOneB(PStr) ;
	}

	// �I��
	return 0 ;
}

// ���b�V�����_�J���[�̉��
static int TempMeshVertexColors( X_PSTRING *PStr, X_MODEL *Model, X_MESHVERTEXCOLORS *MeshVertexColors )
{
	int i ;

	// �J���[�̐����擾
	TempDword( PStr, Model, ( X_DWORD * )&MeshVertexColors->nVertexColors ) ; 	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	// �J���[���i�[���邽�߂̃������̊m��
	if( ( MeshVertexColors->vertexColors = ( X_INDEXEDCOLOR * )ADDMEMAREA( sizeof( X_INDEXEDCOLOR ) * MeshVertexColors->nVertexColors, &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xab\x30\xe9\x30\xfc\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���_�J���[���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// ���_�J���[�̉��
	for( i = 0 ; i < ( int )MeshVertexColors->nVertexColors ; i ++ )
	{
		TempIndexedColor( PStr, Model, &MeshVertexColors->vertexColors[ i ] ) ;
	 	if( !PStr->binf )
		{
			if( PSTRC( PStr ) != ';' && PSTRC( PStr ) != ',' )
			{
				MeshVertexColors->vertexColors[ i ].indexColor.red   *= 255.0f ;
				MeshVertexColors->vertexColors[ i ].indexColor.green *= 255.0f ;
				MeshVertexColors->vertexColors[ i ].indexColor.blue  *= 255.0f ;
				MeshVertexColors->vertexColors[ i ].indexColor.alpha *= 255.0f ;
			}
			else
			{
				PStrMoveP( PStr, ";," ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// ���b�V���e���v���[�g�̉��
static int TempMesh( X_PSTRING *PStr, X_MODEL *Model, X_MESH *Mesh )
{
	DWORD i, j, k ;
	X_VECTOR *V ;
	X_MESHFACE *MF ;

	// ���_�̐����擾����
	TempDword( PStr, Model, ( X_DWORD * )&Mesh->nVertices ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	// ���_���i�[���郁�����̊m��
	if( ( Mesh->vertices = ( X_VECTOR * )ADDMEMAREA( sizeof( X_VECTOR ) * Mesh->nVertices, &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\xa7\x5e\x19\x6a\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���_���W���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// ���_�̐������C���f�b�N�X��ǂݍ���
	if( !PStr->binf )
	{
		if( Mesh->nVertices != 0 )
		{
			V = Mesh->vertices ;
			for( i = 0 ; i < Mesh->nVertices - 1 ; i ++, V ++ )
			{
				TempVector( PStr, Model, V ) ;
				if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
			}
			TempVector( PStr, Model, V ) ;
			if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
		}
	}
	else
	{
		if( PSTRWORD(PStr, 0) != TOKEN_FLOAT_LIST )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : Mesh FloadList\n" )) ;
			return -1 ;
		}
		if( PSTRDWORD(PStr, 2) != Mesh->nVertices * 3 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : Mesh Not %d ( %d )\n", Mesh->nVertices * 3, PSTRDWORD(PStr, 2) ) ) ;
			return -1 ;
		}

		V = Mesh->vertices ;
		j = 6 ;
		if( PStr->f64f )
		{
			for( i = 0 ; i < Mesh->nVertices  ; i ++, V ++, j += 24 )
			{
				V->dat.x = (float)PSTRDOUBLE(PStr,j) ; V->dat.y = (float)PSTRDOUBLE(PStr,j+8) ; V->dat.z = (float)PSTRDOUBLE(PStr,j+16) ;
			}
		}
		else
		{
			for( i = 0 ; i < Mesh->nVertices  ; i ++, V ++, j += 12 )
			{
				V->dat.x = PSTRFLOAT(PStr,j) ; V->dat.y = PSTRFLOAT(PStr,j+4) ; V->dat.z = PSTRFLOAT(PStr,j+8) ;
			}
		}
		PStrMoveOneB( PStr ) ;
	}

	// ���_�t�F�C�X�̐����擾����
	if( !PStr->binf )
	{
		TempDword( PStr, Model, ( X_DWORD * )&Mesh->nFaces ) ;
		PStrMoveP( PStr, ";," ) ;
	}
	else
	{
		switch( PSTRWORD(PStr,0) )
		{
		case TOKEN_INTEGER :		Mesh->nFaces = PSTRDWORD(PStr,2) ; break ;
		case TOKEN_INTEGER_LIST :	Mesh->nFaces = PSTRDWORD(PStr,6) ; break ;
		}
	}

	// ���_�t�F�C�X���i�[���郁�����̊m��
	if( ( Mesh->faces = ( X_MESHFACE * )ADDMEMAREA( sizeof( X_MESHFACE ) * Mesh->nFaces, &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x02\x98\xb9\x70\x62\x97\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���_�ʏ����i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}
	Mesh->nTriangles = 0 ;

	// ���_�t�F�C�X�̐������t�F�C�X��ǂݍ���
	if( !PStr->binf )
	{
		if( Mesh->nFaces != 0 )
		{
			MF = Mesh->faces ;
			for( i = 0 ; i < Mesh->nFaces - 1 ; i ++, MF ++ )
			{
				if( !PStr->binf ) PStrMoveNum( PStr );
				if( TempMeshFace( PStr, Model, MF ) < 0 )
				{
					return -1 ;
				}
				Mesh->nTriangles += MF->nFaceVertexIndices == 4 ? 2 : 1 ;
			}
			if( !PStr->binf ) PStrMoveNum( PStr );
			if( TempMeshFace( PStr, Model, MF ) < 0 )
			{
				return -1 ;
			}
			Mesh->nTriangles += MF->nFaceVertexIndices == 4 ? 2 : 1 ;
		}
	}
	else
	{
//		if( PSTRWORD(PStr, 0) != TOKEN_INTEGER_LIST ) return DXST_ERRORLOG_ADDW( L"Error aadfa;bb" ) ;

		j = 0 ;
		switch( PSTRWORD(PStr,0) )
		{
		case TOKEN_INTEGER : PStrMoveOneB( PStr ) ; j = 6 ; break ;
		case TOKEN_INTEGER_LIST : j = 10 ; break ;
		}

		MF = Mesh->faces ;
		for( i = 0 ; i < Mesh->nFaces ; i ++, MF ++ )
		{
			MF->nFaceVertexIndices = PSTRDWORD(PStr,j) ; j += 4 ;
			if( MF->faceVertexIndices != NULL )
			{
				for( k = 0 ; k < MF->nFaceVertexIndices ; k ++, j += 4 )
				{
					MF->faceVertexIndices[k] = PSTRDWORD(PStr,j) ; 
				}
			}
			else
			{
				for( k = 0 ; k < MF->nFaceVertexIndices ; k ++, j += 4 )
				{
					MF->faceVertexIndicesFixedLength[k] = PSTRDWORD(PStr,j) ; 
				}
			}
			Mesh->nTriangles += MF->nFaceVertexIndices == 4 ? 2 : 1 ;
		}
		PStrMoveOneB( PStr ) ;
	}

	// �I��
	return 0 ;
}

// �t���[��(�y�т��̂��ׂĂ̂��I�u�W�F�N�g)�̃��[�J���g�����X�t�H�[���e���v���[�g�̉��
static int TempFrameTransformMatrix( X_PSTRING *PStr, X_MODEL *Model, X_FRAMETRANSFORMMATRIX *FrameTransformMatrix )
{
	// �s��𓾂�
	if( !PStr->binf )
	{
		TempMatrix4x4( PStr, Model, &FrameTransformMatrix->frameMatrix ) ;
		if( !PStr->binf )
		{
			PStrMoveP( PStr, ";,}" ) ;

			// �ςȋL�q�p�̃v���O����
			if( PStr->StrBuf[ PStr->StrOffset - 1 ] == '}' )
			{
				PStr->StrOffset -- ;
			}
		}
	}
	else
	{
		DWORD i, j, k ;
		X_MATRIX4X4 *mt ;

		if( PSTRWORD(PStr,0) != TOKEN_FLOAT_LIST )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : FrameTransformMatrix FloadList\n" )) ;
			return -1 ;
		}

		mt = &FrameTransformMatrix->frameMatrix ;
		if( PSTRDWORD(PStr,2) == 16 )
		{
			k = 6 ;
			if( PStr->f64f )
			{
				for( i = 0 ; i < 4 ; i ++ )
					for( j = 0 ; j < 4 ; j ++, k += 8 )
						mt->matrix[i][j] = PSTRDOUBLE(PStr,k) ;
			}
			else
			{
				for( i = 0 ; i < 4 ; i ++ )
					for( j = 0 ; j < 4 ; j ++, k += 4 )
						mt->matrix[i][j] = PSTRFLOAT(PStr,k) ;
			}
			PStrMoveOneB( PStr ) ;
		}
		else
		if( PSTRDWORD(PStr,2) == 4 )
		{
			for( i = 0 ; i < 4 ; i ++ )
			{
				if( PSTRWORD(PStr,0) != TOKEN_FLOAT_LIST )
				{
					DXST_ERRORLOGFMT_ADDW(( L"Load XFile : FrameTransformMatrix MatrixLine No.%d FloadList\n", i ) ) ;
					return -1 ;
				}
				if( PSTRDWORD(PStr,2) != 4 )
				{
					DXST_ERRORLOGFMT_ADDW(( L"Load XFile : FrameTransformMatrix MatrixLine No.%d Not 4 ( %d )\n", i, PSTRDWORD(PStr, 2) ) ) ;
					return -1 ;
				}

				k = 6 ;
				for( j = 0 ; j < 4 ; j ++, k += 4 )
					mt->matrix[i][j] = PSTRFLOAT(PStr,k) ;
				PStrMoveOneB( PStr ) ;
			}
		}
		else
		{
			DXST_ERRORLOGFMT_ADDA(( "Load XFile : FrameTransformMatrix Not 16 ( %d )\n", PSTRDWORD(PStr, 2) ) ) ;
			return -1 ;
		}
	}

	// �I��
	return 0 ;
}

// �t���[���e���v���[�g�̉��
static int TempFrame( X_PSTRING * /*PStr*/, X_MODEL * /*Model*/, X_FRAME * /*Frame*/ )
{
	// ���ɂ��邱�ƂȂ�

	// �I��
	return 0 ;
}

// ���������_�̔z��y�єz����� float �̐��e���v���[�g�̉��
static int TempFloatKeys( X_PSTRING *PStr, X_MODEL *Model, X_FLOATKEYS *FloatKeys )
{
	float *f ;
	DWORD i ;

	// float �l�̐����擾
	TempDword( PStr, Model, ( X_DWORD * )&FloatKeys->nValues ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

	if( FloatKeys->nValues == 0 )
	{
		i = 8 ;
	}

	// float �l���i�[���郁�����̊m��
	if( ( FloatKeys->values = ( float * )ADDMEMAREA( sizeof( float ) * FloatKeys->nValues, &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\xea\x30\xb9\x30\xc8\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���������_���X�g���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	if( !PStr->binf )
	{
		// float �l�̐������J��Ԃ�
		if( FloatKeys->nValues != 0 )
		{
			f = FloatKeys->values ;
			for( i = 0 ; i < FloatKeys->nValues - 1 ; i ++, f ++ )
			{
				TempFloat( PStr, Model, ( X_FLOAT * )f ) ;
				if( !PStr->binf ) PStrMove( PStr, "," ) ;
			}
			TempFloat( PStr, Model, ( X_FLOAT * )f ) ;
			if( !PStr->binf ) PStrMove( PStr, ";," ) ;
		}
	}
	else
	{
		DWORD j ;

		PStrMoveOneB(PStr) ;

		// float �l�̐������J��Ԃ�
		j = 6 ;
		if( PStr->f64f )
		{
			f = FloatKeys->values ;
			for( i = 0 ; i < FloatKeys->nValues ; i ++, f ++, j += 8 )
				*f = PSTRDOUBLE(PStr,j) ;
		}
		else
		{
			f = FloatKeys->values ;
			for( i = 0 ; i < FloatKeys->nValues ; i ++, f ++, j += 4 )
				*f = PSTRFLOAT(PStr,j) ;
		}

		PStrMoveOneB(PStr) ;
	}

	// �I��
	return 0 ;
}

// �A�j���[�V�����Ŏg�p����镂�������_���̃Z�b�g�y�ю��Ԃ��`����e���v���[�g�̉��
static int TempTimedFloatKeys( X_PSTRING *PStr, X_MODEL *Model, X_TIMEDFLOATKEYS *TimedFloatKeys )
{
	DWORD i ;
	float *f ;

	if( !PStr->binf )
	{
		// ���Ԓl���擾
		TempDword( PStr, Model, ( X_DWORD * )&TimedFloatKeys->time ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

		// FloatKeys ���擾
		TempFloatKeys( PStr, Model, &TimedFloatKeys->tfkeys ) ;			if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	}
	else
	{
		// ���Ԓl���擾
		TimedFloatKeys->time = PSTRDWORD(PStr,6) ;

		X_FLOATKEYS *FloatKeys = &TimedFloatKeys->tfkeys ;

		// float �l�̐����擾
		FloatKeys->nValues = PSTRDWORD(PStr,10) ;

		// float �l���i�[���郁�����̊m��
		if( ( FloatKeys->values = ( float * )ADDMEMAREA( sizeof( float ) * FloatKeys->nValues, &Model->XModelMem ) ) == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\xad\x30\xfc\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : ���������_�L�[�f�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			return -1 ;
		}

		PStrMoveOneB(PStr) ;

		if( PStr->binf )
		{
			// float �l�̐������J��Ԃ�
			if( FloatKeys->nValues != 0 )
			{
				f = FloatKeys->values ;
				for( i = 0 ; i < FloatKeys->nValues - 1 ; i ++, f ++ )
				{
					TempFloat( PStr, Model, ( X_FLOAT * )f ) ;
					if( !PStr->binf ) PStrMove( PStr, "," ) ;
				}
				TempFloat( PStr, Model, ( X_FLOAT * )f ) ;
				if( !PStr->binf ) PStrMove( PStr, ";," ) ;
			}
		}
		else
		{
			DWORD j ;

			// float �l�̐������J��Ԃ�
			j = 6 ;
			if( PStr->f64f )
			{
				f = FloatKeys->values ;
				for( i = 0 ; i < FloatKeys->nValues ; i ++, f ++, j += 8 )
					*f = PSTRDOUBLE(PStr,j) ;
			}
			else
			{
				f = FloatKeys->values ;
				for( i = 0 ; i < FloatKeys->nValues ; i ++, f ++, j += 4 )
					*f = PSTRFLOAT(PStr,j) ;
			}
		}
		PStrMoveOneB(PStr) ;
	}

	// �I��
	return 0 ;
}

// �A�j���[�V�����L�[�̃Z�b�g�e���v���[�g�̉��
static int TempAnimationKey( X_PSTRING *PStr, X_MODEL *Model, X_ANIMATIONKEY *AnimationKey )
{
	X_TIMEDFLOATKEYS *T ;
	DWORD i, j ;

	if( !PStr->binf )
	{
		// �L�[�^�C�v���擾
		TempDword( PStr, Model, ( X_DWORD * )&AnimationKey->keyType ) ;	if( !PStr->binf ) PStrMove( PStr, ";," ) ;
		
		// �L�[�̐����擾
		TempDword( PStr, Model, ( X_DWORD * )&AnimationKey->nKeys ) ;		if( !PStr->binf ) PStrMove( PStr, ";," ) ;
	}
	else
	{
		// �L�[�^�C�v���擾
		AnimationKey->keyType = PSTRDWORD(PStr,6) ;
		
		// �L�[�̐����擾
		AnimationKey->nKeys = PSTRDWORD(PStr,10) ;
	}
		
	// �L�[��ۑ����郁�����̈�̊m��
	if( ( AnimationKey->keys = ( X_TIMEDFLOATKEYS * )ADDMEMAREA( sizeof( X_TIMEDFLOATKEYS ) * AnimationKey->nKeys, &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\xea\x30\xb9\x30\xc8\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �A�j���[�V�����L�[���X�g���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	if( !PStr->binf )
	{
		// �L�[�̐������L�[��ǂݍ���
		if( AnimationKey->nKeys != 0 )
		{
			T = AnimationKey->keys ;
			for( i = 0 ; i < AnimationKey->nKeys - 1 ; i ++, T ++ )
			{
				TempTimedFloatKeys( PStr, Model, T ) ;
				if( !PStr->binf ) PStrMove( PStr, ";," ) ;
			}
			TempTimedFloatKeys( PStr, Model, T ) ;
			if( !PStr->binf ) PStrMove( PStr, ";," ) ;
		}
	}
	else
	{
		// �L�[�̐������L�[��ǂݍ���
		T = AnimationKey->keys ;
		j = 14 ;
		for( i = 0 ; i < AnimationKey->nKeys ; i ++, T ++ )
		{
			// TimedFloatKeys ���擾
			{
				X_TIMEDFLOATKEYS *TimedFloatKeys = T ;

				// ���Ԓl���擾
				TimedFloatKeys->time = PSTRDWORD(PStr,j) ; j += 4 ;

				// FloatKeys ���擾
				{
					X_FLOATKEYS *FloatKeys = &TimedFloatKeys->tfkeys ;

					float *f ;
					DWORD i ;

					// float �l�̐����擾
					FloatKeys->nValues = PSTRDWORD(PStr,j) ; j += 4 ;

					PStrMoveOneB(PStr) ;
					j = 6 ;

					// float �l���i�[���郁�����̊m��
					if( ( FloatKeys->values = ( float * )ADDMEMAREA( sizeof( float ) * FloatKeys->nValues, &Model->XModelMem ) ) == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xa2\x30\xcb\x30\xe1\x30\xfc\x30\xb7\x30\xe7\x30\xf3\x30\xad\x30\xfc\x30\x6e\x30\x6e\x6d\xd5\x52\x0f\x5c\x70\x65\xb9\x70\x24\x50\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �A�j���[�V�����L�[�̕��������_�l���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
						return -1 ;
					}

					// float �l�̐������J��Ԃ�
					if( !PStr->binf )
					{
						f = FloatKeys->values ;
						for( i = 0 ; i < FloatKeys->nValues ; i ++, f ++, j += 8 )
							*f = PSTRDOUBLE(PStr,j) ;
					}
					else
					{
						f = FloatKeys->values ;
						for( i = 0 ; i < FloatKeys->nValues ; i ++, f ++, j += 4 )
							*f = PSTRFLOAT(PStr,j) ;
					}
				}
			}
			PStrMoveOneB(PStr) ;
			j = 6 ;
		}
	}

	// �I��
	return 0 ;
}

// �A�j���[�V�����I�v�V�����e���v���[�g�̉��
static int TempAnimationOptions( X_PSTRING *PStr, X_MODEL *Model, X_ANIMATIONOPTIONS *AnimationOptions )
{
	if( !PStr->binf )
	{
		// �A�j���[�V���������Ă��邩�ǂ������擾����
		TempDword( PStr, Model, ( X_DWORD * )&AnimationOptions->openclosed ) ;		if( !PStr->binf ) PStrMove( PStr, ";," ) ;
		
		// �ʒu�L�[�̈ʒu�̕i���l���擾����
		TempDword( PStr, Model, ( X_DWORD * )&AnimationOptions->positionquality ) ;	if( !PStr->binf ) PStrMove( PStr, ";," ) ;
	}
	else
	{
		// �A�j���[�V���������Ă��邩�ǂ������擾����
		AnimationOptions->openclosed = PSTRDWORD(PStr,6) ;
		
		// �ʒu�L�[�̈ʒu�̕i���l���擾����
		AnimationOptions->positionquality = PSTRDWORD(PStr,10) ;

		PStrMoveOneB(PStr) ;
	}

	// �I��
	return 0 ;
}

// �A�j���[�V�����e���v���[�g�̉��
static int TempAnimation( X_PSTRING * /*PStr*/, X_MODEL * /*Model*/, X_ANIMATION * /*Animation*/ )
{
	// ���ɂ��邱�ƂȂ�

	// �I��
	return 0 ;
}

// �A�j���[�V�����Z�b�g�e���v���[�g�̉��
static int TempAnimationSet( X_PSTRING * /*PStr*/, X_MODEL * /*Model*/, X_ANIMATIONSET * /*AnimationSet*/ )
{
	// ���ɂ��邱�ƂȂ�

	// �I��
	return 0 ;
}

// �X�L�j���O���b�V���̃w�b�_���e���v���[�g�̉��
static int TempXSkinMeshHeader( X_PSTRING *PStr, X_MODEL *Model, X_XSKINMESHHEADER *XSkinMeshHeader )
{
	if( !PStr->binf )
	{
		// �{�[����ɑ΂��čő傢���̒��_���ւ�邩�̐����擾
		TempWord( PStr, Model, ( X_WORD * )&XSkinMeshHeader->nMaxSkinWeightsPerVertex ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

		// �{�[����ɑ΂��čő傢���̖ʂ��ւ�邩�̐����擾
		TempWord( PStr, Model, ( X_WORD * )&XSkinMeshHeader->nMaxSkinWeightsPerFace ) ;	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;

		// �{�[���̐����擾
		TempWord( PStr, Model, ( X_WORD * )&XSkinMeshHeader->nBones ) ;					if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	}
	else
	{
		if( PSTRWORD(PStr,0) != TOKEN_INTEGER_LIST )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : XSkinMeshHeader IntergerList\n" )) ;
			return -1 ;
		}
		if( PSTRDWORD(PStr,2) != 3 )
		{
			DXST_ERRORLOGFMT_ADDA(( "Load XFile : XSkinMeshHeader Not 3 ( %d )\n", PSTRDWORD(PStr,2) ) ) ;
			return -1 ;
		}

		// �{�[����ɑ΂��čő傢���̒��_���ւ�邩�̐����擾
		XSkinMeshHeader->nMaxSkinWeightsPerVertex = ( WORD )PSTRDWORD(PStr,6) ;

		// �{�[����ɑ΂��čő傢���̖ʂ��ւ�邩�̐����擾
		XSkinMeshHeader->nMaxSkinWeightsPerFace = ( WORD )PSTRDWORD(PStr,10) ;

		// �{�[���̐����擾
		XSkinMeshHeader->nBones = ( WORD )PSTRDWORD(PStr,14) ;

		PStrMoveOneB( PStr ) ;
	}

	// �I��
	return 0 ;
}

// �X�L�����b�V���̃E�G�C�g���e���v���[�g�̉��
static int TempSkinWeights( X_PSTRING *PStr, X_MODEL *Model, X_SKINWEIGHTS *SkinWeights )
{
	DWORD i ;
	DWORD *D ;
	float *F ;

	// �e�����󂯂�{�[�������擾����
	TempString( PStr, Model, &SkinWeights->transformNodeName ) ;
	if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	
	// �E�G�C�g���󂯂钸�_�̐����擾
	if( !PStr->binf )
	{
		TempDword( PStr, Model, ( X_DWORD * )&SkinWeights->nWeights ) ;
		if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	}
	else
	{
		SkinWeights->nWeights = PSTRDWORD(PStr,6) ;
	}

	// ���_�̃C���f�b�N�X���i�[���郁�����̈�ƃE�G�C�g�l���i�[���郁�����̈���m��
	if( ( SkinWeights->vertexIndices = ( DWORD * )ADDMEMAREA( sizeof( DWORD ) * ( SkinWeights->nWeights + 1 ), &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xf3\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\x6e\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\x24\x50\xc5\x60\x31\x58\x6e\x30\xbf\x30\xfc\x30\xb2\x30\xc3\x30\xc8\x30\x02\x98\xb9\x70\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �X�L�����b�V���̃E�G�C�g�l���̃^�[�Q�b�g���_�C���f�b�N�X���i�[���邽�߂̃������̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	if( ( SkinWeights->weights = ( float * )ADDMEMAREA( sizeof( float ) * ( SkinWeights->nWeights + 1 ), &Model->XModelMem ) ) == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x4c\x00\x6f\x00\x61\x00\x64\x00\x20\x00\x58\x00\x46\x00\x69\x00\x6c\x00\x65\x00\x20\x00\x3a\x00\x20\x00\xb9\x30\xad\x30\xf3\x30\xe1\x30\xc3\x30\xb7\x30\xe5\x30\x6e\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\x24\x50\xc5\x60\x31\x58\x6e\x30\xa6\x30\xa8\x30\xa4\x30\xc8\x30\x24\x50\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\x5f\x30\x81\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"Load XFile : �X�L�����b�V���̃E�G�C�g�l���̃E�G�C�g�l���i�[���邽�߂̃������̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	if( !PStr->binf )
	{
		// �E�G�C�g���󂯂钸�_�̐��������_�̃C���f�b�N�X�l�𓾂�
		if( SkinWeights->nWeights > 0 )
		{
			D = SkinWeights->vertexIndices ;
			for( i = 0 ; i < SkinWeights->nWeights - 1 ; i ++, D ++ )
			{
				TempDword( PStr, Model, ( X_DWORD * )D ) ;
				if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
			}
			TempDword( PStr, Model, ( X_DWORD * )D ) ;
			if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
		}

		// �E�G�C�g���󂯂钸�_�̐��������_�̃E�G�C�g�l�𓾂�
		if( SkinWeights->nWeights > 0 )
		{
			F = SkinWeights->weights ;
			for( i = 0 ; i < SkinWeights->nWeights - 1 ; i ++, F ++ )
			{
				TempFloat( PStr, Model, ( X_FLOAT * )F ) ;
				if( !PStr->binf ) PStrMoveP( PStr, "," ) ;
			}
			TempFloat( PStr, Model, ( X_FLOAT * )F ) ;
			if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
		}

		// �I�t�Z�b�g�s����擾����
		TempMatrix4x4( PStr, Model, &SkinWeights->matrixOffset ) ; if( !PStr->binf ) PStrMoveP( PStr, ";," ) ;
	}
	else
	{
		DWORD j, k ;
		X_MATRIX4X4 *mt ;

		if( PSTRWORD(PStr,0) != TOKEN_INTEGER_LIST )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : SkinWeights TargetVertex IntegerList\n" )) ;
			return -1 ;
		}
		if( PSTRWORD(PStr,2) != 1 + SkinWeights->nWeights )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : SkinWeights TargetVertex Not %d ( %d )\n", 1 + SkinWeights->nWeights, PSTRDWORD(PStr,2) ) ) ;
			return -1 ;
		}

		k = 10 ;
		// �E�G�C�g���󂯂钸�_�̐��������_�̃C���f�b�N�X�l�𓾂�
		D = SkinWeights->vertexIndices ;
		for( i = 0 ; i < SkinWeights->nWeights ; i ++, D ++, k += 4 )
			*D = PSTRDWORD(PStr,k) ;

		PStrMoveOneB( PStr ) ;

		if( PSTRWORD(PStr,0) != TOKEN_FLOAT_LIST )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : SkinWeights WeightValue FloatList\n" )) ;
			return -1 ;
		}

		if( PSTRWORD(PStr,2) != 16 + SkinWeights->nWeights )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Load XFile : SkinWeights WeightValue Not %d ( %d )\n", 16 + SkinWeights->nWeights, PSTRDWORD(PStr,2) ) ) ;
			return -1 ;
		}

		k = 6 ;
		if( PStr->f64f )
		{
			// �E�G�C�g���󂯂钸�_�̐��������_�̃E�G�C�g�l�𓾂�
			F = SkinWeights->weights ;
			for( i = 0 ; i < SkinWeights->nWeights ; i ++, F ++, k += 8 )
				*F = PSTRDOUBLE(PStr,k) ;

			// �I�t�Z�b�g�s����擾����
			mt = &SkinWeights->matrixOffset ;
			for( i = 0 ; i < 4 ; i ++ )
				for( j = 0 ; j < 4 ; j ++, k += 8 )
					mt->matrix[i][j] = PSTRDOUBLE(PStr,k) ;
		}
		else
		{
			// �E�G�C�g���󂯂钸�_�̐��������_�̃E�G�C�g�l�𓾂�
			F = SkinWeights->weights ;
			for( i = 0 ; i < SkinWeights->nWeights ; i ++, F ++, k += 4 )
				*F = PSTRFLOAT(PStr,k) ;

			// �I�t�Z�b�g�s����擾����
			mt = &SkinWeights->matrixOffset ;
			for( i = 0 ; i < 4 ; i ++ )
				for( j = 0 ; j < 4 ; j ++, k += 4 )
					mt->matrix[i][j] = PSTRFLOAT(PStr,k) ;
		}

		PStrMoveOneB( PStr ) ;
	}

	// �I��
	return 0 ;
}














// �w�t�@�C����ǂݍ���( -1:�G���[  0�ȏ�:���f����f�[�^�n���h�� )
extern int MV1LoadModelToX( const MV1_MODEL_LOAD_PARAM *LoadParam, int ASyncThread )
{
	X_MODEL XModel ;
	int NewHandle = -1 ;
	MV1_MODEL_R RModel ;

	// �ǂݍ��ݗp���f���̏�����
	MV1InitReadModel( &RModel ) ;

	// ���f�����ƃt�@�C�����ƕ����R�[�h�`�����Z�b�g
	RModel.CharCodeFormat = CHAR_CHARCODEFORMAT ;
	RModel.FilePath = ( wchar_t * )DXALLOC( ( _WCSLEN( LoadParam->FilePath ) + 1 ) * sizeof( wchar_t ) ) ;
	RModel.Name     = ( wchar_t * )DXALLOC( ( _WCSLEN( LoadParam->Name     ) + 1 ) * sizeof( wchar_t ) ) ;
	_WCSCPY( RModel.FilePath, LoadParam->FilePath ) ;
	_WCSCPY( RModel.Name,     LoadParam->Name ) ;

	// �w�t�@�C���f�[�^�̓ǂݍ���
	{
		// �w�t�@�C����͗p�f�[�^�̏�����
		_MEMSET( &XModel, 0, sizeof( XModel ) ) ;

		// �f�[�^�\���̍ŏ㋉�f�[�^��ǉ�����
		AddObject( TEMP_FRAME, "XTopFrame", NULL, &XModel ) ;

		// �w�t�@�C���̉��
		if( AnalysXFile( ( char * )LoadParam->DataBuffer, LoadParam->DataSize, &XModel ) < 0 ) goto ERRORLABEL ;

		// �w�t�@�C���̉�́A�Q�p�X�ڂ݂����Ȃ���
		if( AnalysXData( &XModel, &RModel ) < 0 ) goto ERRORLABEL ;
	}

	// X�t�@�C���̃f�[�^�����
	TerminateXModel( &XModel ) ;

	// �ǂݍ��݃��f�������{���f���f�[�^���쐬����
	NewHandle = MV1LoadModelToReadModel( &LoadParam->GParam, &RModel, LoadParam->CurrentDir, LoadParam->FileReadFunc, ASyncThread ) ;
	if( NewHandle == -1 ) goto ERRORLABEL ;

	// �ǂݍ��݃��f���̌�n��
	MV1TermReadModel( &RModel ) ;

	// �n���h����Ԃ�
	return NewHandle ;

ERRORLABEL :

	// �ǂݍ��݃��f���̌�n��
	MV1TermReadModel( &RModel ) ;

	// ��{���f���f�[�^�̉��
	if( NewHandle != -1 )
	{
		MV1SubModelBase( NewHandle ) ;
		NewHandle = -1 ;
	}

	// X�t�@�C���̃f�[�^�����
	TerminateXModel( &XModel ) ;

	// �G���[�I��
	return -1 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif

