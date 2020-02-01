// This code is part of example that was prepared specially for https://github.com/3F/Examples
// Copyright (c) 2018,2020  Denis Kuzmin < x-3F@outlook.com > GitHub/3F
// Distributed under the MIT License

#include "pch.h"
#include "PEModule.h"
#include "AlgVB.h"
#include "StdDevTool.h"
#include "LibraryException.h"

using namespace std;
using namespace examples::dllexport::basicExport;

// graph helper for illustration, keep scrolling to PEModule use ...
SHORT _tgraph(PEModule& lib, LPCSTR proc, SHORT dy = 2)
{
    int r1      = 5,    r2      = 70;
    int ofsY    = r1 ,  ofsX    = 60;

    StdDevTool sdt;
    int scale       = 1;
    COORD cursor    = sdt.getCursor();

    for(int y = r1; y < r2; ++y)
    {
        double xi = lib.call<double>(proc, y, 24.781, 27.0);

        sdt.setCursor(0, cursor.Y++);
        printf_s("    \t(%d, 24.781, 27.0) == %e\n", y, xi);

        int ox = (int)(xi / scale) + ofsX;
        int oy = y + dy;

        sdt.write(_T("#"), ox, oy - ofsY);
        sdt.write(_T("*"), ox, oy);
    }
    sdt.setCursor(0, cursor.Y);

    return cursor.Y;
}

int main()
{
    cout << ".NET DllExport Examples between Unmanaged Native C++ and .NET Framework + .NET Core" << endl;
    cout << "https://github.com/3F/Examples" << endl;
    cout << "= = = - = = =" << endl << endl;

    try
    {
        // ################## .NET ClassLibrary1.dll (netfx; C#) #######################

        PEModule lib(_T("ClassLibrary1.dll"));
        cout << ".NET ClassLibrary1.dll (netfx; C#) is ready for requests:\n\n";        

        /* In the manner of Conari syntax: https://github.com/3F/Conari */

        double x1 = lib.call<double, int, double, double>("AmazingSin", 5, 0, 0);
        printf_s("  AmazingSin(5, 0, 0) == %e\n", x1);

        printf_s("    Is NaN(AmazingSin(5, 0, 0)): %s\n", (_isnan(x1) || !_finite(x1)) ? "yes" : "no");

        double x2 = lib.call<double>("AmazingSin", 5, 24.781, (double)11);
        printf_s("  AmazingSin(5, 24.781, 11) == %e\n", x2);
        
        printf_s("    Is NaN(AmazingSin(5, 24.781, 11)): %s\n", (_isnan(x2) || !_finite(x2)) ? "yes" : "no");

        SHORT ofs = _tgraph(lib, "AmazingSin");
        cout << endl;

        
        // ################## C++ Class wrapper of the .NET ClassLibrary2.dll (netfx; visual basic) ##########
        cout << endl;

        AlgVB vb(_T("ClassLibrary2.dll"));
        cout << "C++ Class wrapper of .NET ClassLibrary2.dll (netfx; visual basic) is ready:\n\n";

        printf_s("  AlgVB.superPow(2, 3) == %e\n", vb.superPow(2, 3));

        

        // ################## .NET Core DotNetCoreLibrary3.dll (netstandard2; C#) #######################
        cout << endl;

        PEModule libcore(_T("DotNetCoreLibrary3.dll"));
        cout << ".NET Core DotNetCoreLibrary3.dll (netstandard2; C#) is ready for requests:\n\n";

        printf_s("  AmazingCos(5, 24.781, 11) == %e\n", libcore.call<double>("AmazingCos", 5, 24.781, (double)11));

        _tgraph(libcore, "AmazingCos", ofs + 10);
        cout << endl;

    }
    catch(const LibraryException& ex)
    {
        fprintf_s(stderr, "Error: [ %d ] %s\n", ex.getError(), ex.what());
        return ERROR_INVALID_DATA;
    }
    catch(const exception& ex)
    {
        fprintf_s(stderr, "Error: %s\n", ex.what());
        return ERROR_INVALID_FUNCTION;
    }

    return ERROR_SUCCESS;
}
