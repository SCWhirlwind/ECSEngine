#include "pch.h"
#include "CppUnitTest.h"
#include "../ECSEngine/Vec2.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ECSEngineTest
{
	TEST_CLASS(VectorTest)
	{
	public:

		TEST_METHOD(ZeroVectorTest)
		{
			Vec2 v1;

			Assert::AreEqual(v1.x, 0.0f);
			Assert::AreEqual(v1.y, 0.0f);
		}
		
		TEST_METHOD(AddingVectorsTest)
		{
			Vec2 v1(1.0f, 1.0f);

			Vec2 v2(1.0f, 1.0f);

			Vec2 v3 = v1 + v2;

			Assert::AreEqual(v3.x, 2.0f);
			Assert::AreEqual(v3.y, 2.0f);
		}

		TEST_METHOD(SubtractingVectorsTest)
		{
			Vec2 v1(1.0f, 1.0f);

			Vec2 v2(1.0f, 1.0f);

			Vec2 v3 = v1 - v2;

			Assert::AreEqual(v3.x, 0.0f);
			Assert::AreEqual(v3.y, 0.0f);
		}

		TEST_METHOD(MultiplyingVectorsTest)
		{
			Vec2 v1(1.0f, 1.0f);

			Vec2 v2 = v1 * 2;

			Assert::AreEqual(v2.x, 2.0f);
			Assert::AreEqual(v2.y, 2.0f);
		}

		TEST_METHOD(DividingVectorsTest)
		{
			Vec2 v1(4.0f, 4.0f);

			Vec2 v2 = v1 / 2;

			Assert::AreEqual(v2.x, 2.0f);
			Assert::AreEqual(v2.y, 2.0f);
		}

		TEST_METHOD(AddingToVectorTest)
		{
			Vec2 v1(1.0f, 1.0f);

			Vec2 v2(1.0f, 1.0f);

			v1 += v2;

			Assert::AreEqual(v1.x, 2.0f);
			Assert::AreEqual(v1.y, 2.0f);
		}

		TEST_METHOD(SubtractingToVectorTest)
		{
			Vec2 v1(1.0f, 1.0f);

			Vec2 v2(1.0f, 1.0f);

			v1 -= v2;

			Assert::AreEqual(v1.x, 0.0f);
			Assert::AreEqual(v1.y, 0.0f);
		}

		TEST_METHOD(MultiplyingToVectorTest)
		{
			Vec2 v1(1.0f, 1.0f);

			v1 *= 2;

			Assert::AreEqual(v1.x, 2.0f);
			Assert::AreEqual(v1.y, 2.0f);
		}

		TEST_METHOD(DividingToVectorTest)
		{
			Vec2 v1(4.0f, 4.0f);

			v1 /= 2;

			Assert::AreEqual(v1.x, 2.0f);
			Assert::AreEqual(v1.y, 2.0f);
		}
	};
}
