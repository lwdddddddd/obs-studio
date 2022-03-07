#include "aja-vpid-data.hpp"

VPIDData::VPIDData()
	: mVpidA{0},
	  mVpidB{0},
	  mStandardA{VPIDStandard_Unknown},
	  mStandardB{VPIDStandard_Unknown},
	  mSamplingA{VPIDSampling_XYZ_444},
	  mSamplingB{VPIDSampling_XYZ_444}
{
}

VPIDData::VPIDData(ULWord vpidA, ULWord vpidB)
	: mVpidA{vpidA},
	  mVpidB{vpidB},
	  mStandardA{VPIDStandard_Unknown},
	  mStandardB{VPIDStandard_Unknown},
	  mSamplingA{VPIDSampling_XYZ_444},
	  mSamplingB{VPIDSampling_XYZ_444}
{
	Parse();
}

VPIDData::VPIDData(const VPIDData &other)
	: mVpidA{other.mVpidA},
	  mVpidB{other.mVpidB},
	  mStandardA{VPIDStandard_Unknown},
	  mStandardB{VPIDStandard_Unknown},
	  mSamplingA{VPIDSampling_XYZ_444},
	  mSamplingB{VPIDSampling_XYZ_444}
{
	Parse();
}
VPIDData::VPIDData(VPIDData &&other)
	: mVpidA{other.mVpidA},
	  mVpidB{other.mVpidB},
	  mStandardA{VPIDStandard_Unknown},
	  mStandardB{VPIDStandard_Unknown},
	  mSamplingA{VPIDSampling_XYZ_444},
	  mSamplingB{VPIDSampling_XYZ_444}
{
	Parse();
}

VPIDData &VPIDData::operator=(const VPIDData &other)
{
	mVpidA = other.mVpidA;
	mVpidB = other.mVpidB;
	return *this;
}

VPIDData &VPIDData::operator=(VPIDData &&other)
{
	mVpidA = other.mVpidA;
	mVpidB = other.mVpidB;
	return *this;
}

bool VPIDData::operator==(const VPIDData &rhs) const
{
	return (mVpidA == rhs.mVpidA && mVpidB == rhs.mVpidB);
}

bool VPIDData::operator!=(const VPIDData &rhs) const
{
	return !operator==(rhs);
}

void VPIDData::SetA(ULWord vpidA)
{
	mVpidA = vpidA;
}

void VPIDData::SetB(ULWord vpidB)
{
	mVpidB = vpidB;
}

void VPIDData::Parse()
{
	CNTV2VPID parserA;
	parserA.SetVPID(mVpidA);
	mStandardA = parserA.GetStandard();
	mSamplingA = parserA.GetSampling();

	CNTV2VPID parserB;
	parserB.SetVPID(mVpidB);
	mStandardB = parserB.GetStandard();
	mSamplingB = parserB.GetSampling();
}

bool VPIDData::IsRGB() const
{
	switch (mSamplingA) {
	default:
		break;
	case VPIDSampling_GBR_444:
	case VPIDSampling_GBRA_4444:
	case VPIDSampling_GBRD_4444:
		return true;
	}
	return false;
}

VPIDStandard VPIDData::Standard() const
{
	return mStandardA;
}

VPIDSampling VPIDData::Sampling() const
{
	return mSamplingA;
}
