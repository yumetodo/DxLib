// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�o�[�e�b�N�X�V�F�[�_�[�֘A��`
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// Float4 �^�萔
#define DX_VS_CONSTF_ZERO_ONE                          0			// x:0.0f y:1.0f
#define DX_VS_CONSTF_AMBIENT_EMISSIVE                  1			// �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[
#define DX_VS_CONSTF_PROJECTION_MAT                    2			// �ˉe�s��̓]�u�s��̒萔�A�h���X
#define DX_VS_CONSTF_VIEW_MAT                          6			// �r���[�s��̓]�u�s��̒萔�A�h���X
#define DX_VS_CONSTF_FOG                               10			// �t�H�O�p�p�����[�^���i�[����A�h���X( x:end/(end - start)  y:-1/(end - start)  z:density  w:���R�ΐ��̒� )

#define DX_VS_CONSTF_MATERIAL_START                    11			// �}�e���A���̃p�����[�^���i�[�����萔�A�h���X
#define DX_VS_CONSTF_MATERIAL_SIZE                     3			// �}�e���A���p�̃��W�X�^�̐�
#define DX_VS_CONSTF_MAT_DIFFUSE                       0			// �}�e���A���̃f�B�t�[�Y�F
#define DX_VS_CONSTF_MAT_SPECULAR                      1			// �}�e���A���̃X�y�L�����[�F
#define DX_VS_CONSTF_MAT_POWER                         2			// �}�e���A���̃X�y�L�����n�C���C�g�̃p���[

#define DX_VS_CONSTF_LIGHT_START                       14			// ���C�g�̃p�����[�^���i�[�����萔�A�h���X
#define DX_VS_CONSTF_LIGHT_NUM                         4			// �g���C�A���O�����X�g��œ����Ɏg�p���郉�C�g�̍ő吔
#define DX_VS_CONSTF_LIGHT_UNITSIZE                    7			// ���C�g�p�����[�^��ӂ�Ɏg�p���郌�W�X�^�̐�
#define DX_VS_CONSTF_LGT_POSITION                      0			// ���C�g�̈ʒu( �r���[��� )
#define DX_VS_CONSTF_LGT_DIRECTION                     1			// ���C�g�̕���( �r���[��� )
#define DX_VS_CONSTF_LGT_DIFFUSE                       2			// ���C�g�̃f�B�t�[�Y�F
#define DX_VS_CONSTF_LGT_SPECULAR                      3			// ���C�g�̃X�y�L�����F
#define DX_VS_CONSTF_LGT_AMBIENT                       4			// ���C�g�̃A���r�G���g�F�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
#define DX_VS_CONSTF_LGT_RANGE_FALLOFF_AT0_AT1         5			// ���C�g�� x:�L�������̓�� y:�t�H�[���I�t z:���������p�����[�^�O w:���������p�����[�^�P
#define DX_VS_CONSTF_LGT_AT2_SPOTP0_SPOTP1             6			// ���C�g�� x:���������p�����[�^�Q y:�X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) ) z:�X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

#define DX_VS_CONSTF_TOON_OUTLINE_SIZE                 42			// �g�D�[�������_�����O�p�̗֊s���̑���
#define DX_VS_CONSTF_DIF_SPE_SOURCE                    43			// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̃\�[�X  x:�f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )  y:�X�y�L�����J���[( 0.0f:�}�e���A��  1.0f:���_ )
#define DX_VS_CONSTF_SHADOWMAP1_LIGHT_VIEW_PROJECTION_MAT 44		// �V���h�E�}�b�v�P�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
#define DX_VS_CONSTF_SHADOWMAP2_LIGHT_VIEW_PROJECTION_MAT 48		// �V���h�E�}�b�v�Q�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
#define DX_VS_CONSTF_SHADOWMAP3_LIGHT_VIEW_PROJECTION_MAT 52		// �V���h�E�}�b�v�R�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������

#define DX_VS_CONSTF_TEXTURE_MATRIX_START              88			// �e�N�X�`�����W�ϊ��s��̓]�u�s�񂪊i�[�����萔�A�h���X
#define DX_VS_CONSTF_TEXTURE_MATRIX_NUM                3			// �e�N�X�`�����W�ϊ��s��̓]�u�s��̐�
#define DX_VS_CONSTF_TEXTURE_MATRIX_UNITSIZE           2			// �e�N�X�`�����W�ϊ��s��̓]�u�s��������Ɏg�p����郌�W�X�^�̐�

#define DX_VS_CONSTF_WORLD_MAT_START                   94			// ���[�J�������[���h�s��̓]�u�s�񂪊i�[�����萔�A�h���X
#define DX_VS_CONSTF_WORLD_MAT_NUM                     54			// �g���C�A���O�����X�g��œ����Ɏg�p���郍�[�J�������[���h�s��̍ő吔

// Bool �^�萔
#define DX_VS_CONSTB_FOG_LINEAR                        0			// ���`�t�H�O�g�p�̗L��
#define DX_VS_CONSTB_FOG_EXP                           1			// �w���֐��t�H�O�g�p�̗L��
#define DX_VS_CONSTB_FOG_EXP2                          2			// �w���֐��t�H�O�Q�g�p�̗L��
#define DX_VS_CONSTB_FOG_USE                           3			// �t�H�O�g�p�̗L��
#define DX_VS_CONSTB_LIGHT_START                       4			// ���C�g���̊J�n�萔�A�h���X
#define DX_VS_CONSTB_LIGHT_NUM                         4			// �g���C�A���O�����X�g��œ����Ɏg�p���郉�C�g�̍ő吔
#define DX_VS_CONSTB_LIGHT_UNITSIZE                    3			// ���C�g�������Ɏg�p����u�[���^�萔�̐�
#define DX_VS_CONSTB_LGT_USE                           0			// ���C�g�g�p�̗L��
#define DX_VS_CONSTB_LGT_POINT_OR_SPOT                 1			// �|�C���g���C�g���Ⴕ���̓X�|�b�g���C�g���ǂ���
#define DX_VS_CONSTB_LGT_SPOT                          2			// �X�|�b�g���C�g���ǂ���


