CXX = cl
CXXFLAGS = /std:c++20 /EHsc /nologo /Iheaders

CORE = build\Board.obj build\negamax.obj build\alphabeta.obj

all:
	@for %f in (src\*.cpp) do @$(CXX) $(CXXFLAGS) /c %f /Fo:build\ > nul 2>&1

	@$(CXX) build\main.obj $(CORE) /Fe:build\main.exe /Fd:build\main.pdb > nul 2>&1
	@$(CXX) build\benchmark.obj $(CORE) /Fe:build\benchmark.exe /Fd:build\benchmark.pdb > nul 2>&1

	@build\main.exe