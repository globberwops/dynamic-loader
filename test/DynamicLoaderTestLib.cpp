extern "C" void DynamicLoaderTest() {}

extern "C" void DynamicLoaderTestParam(int param) {}

extern "C" auto DynamicLoaderTestReturn() -> int { return 42; }

extern "C" auto DynamicLoaderTestParamReturn(int param) -> int { return param; }

auto DynamicLoaderTestMangled(int param) -> int { return param; }
