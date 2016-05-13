#pragma once

namespace plu{

	// index buffer֧�ֵ�ͼԪ����, ��Ӧ��dx֧�ֵ���������
	enum IPType{
		IP_NONE                     = 0xFFFFFFFF,
		IP_POLYPOINT                = 0x00000001,
		IP_POLYSEGMENT_DISJOINT     = 0x00000002,
		IP_POLYSEGMENT_CONTIGUOUS   = 0x00000004,
		IP_TRIMESH                  = 0x00000008,
		IP_TRISTRIP                 = 0x00000010, //��������ɵ�����

		IP_HAS_POINTS    = IP_POLYPOINT,
		IP_HAS_SEGMENTS  = IP_POLYSEGMENT_DISJOINT | IP_POLYSEGMENT_CONTIGUOUS,
		IP_HAS_TRIANGLES = IP_TRIMESH | IP_TRISTRIP,

		IP_NUM_TYPES = 5 // IP_POLYPOINT through IP_TRISTRIP
	};

}
