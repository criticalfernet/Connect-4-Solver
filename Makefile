CXX = cl

CXXFLAGS = /std:c++20 /EHsc /nologo

all:
	@$(CXX) $(CXXFLAGS) /c src\main.cpp /Fo:build\main.obj > nul
	@$(CXX) $(CXXFLAGS) /c src\Board.cpp /Fo:build\Board.obj > nul
	@$(CXX) $(CXXFLAGS) /c src\negamax.cpp /Fo:build\negamax.obj > nul

	@$(CXX) build\main.obj build\Board.obj build\negamax.obj /Fe:build\main.exe /Fd:build\main.pdb > nul

	@build\main.exe