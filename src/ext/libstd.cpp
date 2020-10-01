#include "../ruota/Ruota.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <boost/generator_iterator.hpp>

#ifndef __unix__
#include <conio.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <termios.h>
#endif

RUOTA_LIB_HEADER

namespace libstd
{
	RUOTA_EXT_SYM(_puts, args, token)
	{
		std::cout << args[0].getString(token);
		return Symbol();
	}

	RUOTA_EXT_SYM(_timeMS, args, token)
	{
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		return Symbol(CNumber(static_cast<long_int_t>(ms.count())));
	}

	RUOTA_EXT_SYM(_rand_init, args, token)
	{
		auto rng = std::make_shared<std::default_random_engine>(args[0].getNumber(token).getLong());
		return Symbol(static_cast<std::shared_ptr<void>>(rng));
	}

	RUOTA_EXT_SYM(_rand_nextFloat, args, token)
	{
		auto rng = static_cast<std::default_random_engine *>(args[0].getPointer(token).get());
		std::uniform_real_distribution<long_double_t> distribution(args[1].getNumber(token).getDouble(), args[2].getNumber(token).getDouble());
		return Symbol(CNumber(static_cast<long_double_t>(distribution(*rng))));
	}

	RUOTA_EXT_SYM(_rand_nextInt, args, token)
	{
		auto rng = static_cast<std::default_random_engine *>(args[0].getPointer(token).get());
		std::uniform_int_distribution<long_int_t> distribution(args[1].getNumber(token).getLong(), args[2].getNumber(token).getLong());
		return Symbol(CNumber(static_cast<long_int_t>(distribution(*rng))));
	}

	RUOTA_EXT_SYM(_exit, args, token)
	{
		exit(args[0].getNumber(token).getLong());
	}

	RUOTA_EXT_SYM(_system_call, args, token)
	{
		return Symbol(CNumber(static_cast<long_int_t>(system(args[0].getString(token).c_str()))));
	}

	RUOTA_EXT_SYM(_sleep, args, token)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds((long long)args[0].getNumber(token).getLong()));
		return Symbol();
	}

	RUOTA_EXT_SYM(_log, args, token)
	{
		return Symbol(CNumber(static_cast<long_double_t>(std::log(args[0].getNumber(token).getDouble()))));
	}

	RUOTA_EXT_SYM(_sin, args, token)
	{
		return Symbol(CNumber(static_cast<long_double_t>(std::sin(args[0].getNumber(token).getDouble()))));
	}

	RUOTA_EXT_SYM(_cos, args, token)
	{
		return Symbol(CNumber(static_cast<long_double_t>(std::cos(args[0].getNumber(token).getDouble()))));
	}

	RUOTA_EXT_SYM(_tan, args, token)
	{
		return Symbol(CNumber(static_cast<long_double_t>(std::tan(args[0].getNumber(token).getDouble()))));
	}

	RUOTA_EXT_SYM(_sinh, args, token)
	{
		return Symbol(CNumber(static_cast<long_double_t>(std::sinh(args[0].getNumber(token).getDouble()))));
	}

	RUOTA_EXT_SYM(_cosh, args, token)
	{
		return Symbol(CNumber(static_cast<long_double_t>(std::cosh(args[0].getNumber(token).getDouble()))));
	}

	RUOTA_EXT_SYM(_tanh, args, token)
	{
		return Symbol(CNumber(static_cast<long_double_t>(std::tanh(args[0].getNumber(token).getDouble()))));
	}

	RUOTA_EXT_SYM(_floor, args, token)
	{
		if (args[0].getNumber(token).getType() == LONG_NUM)
			return args[0];
		return Symbol(CNumber(static_cast<long_int_t>(std::floor(args[0].getNumber(token).getDouble()))));
	}

	RUOTA_EXT_SYM(_ceil, args, token)
	{
		if (args[0].getNumber(token).getType() == LONG_NUM)
			return args[0];
		return Symbol(CNumber(static_cast<long_int_t>(std::ceil(args[0].getNumber(token).getDouble()))));
	}

	RUOTA_EXT_SYM(_round, args, token)
	{
		if (args[0].getNumber(token).getType() == LONG_NUM)
			return args[0];
		return Symbol(CNumber(static_cast<long_int_t>(std::round(args[0].getNumber(token).getDouble()))));
	}

	RUOTA_EXT_SYM(_input_line, args, token)
	{
		std::string line;
		std::getline(std::cin, line);
		return Symbol(line);
	}

	RUOTA_EXT_SYM(_input_char, args, token)
	{
#ifdef __unix__
		struct termios oldattr, newattr;
		int ch;
		tcgetattr(0, &oldattr);
		newattr = oldattr;
		newattr.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(0, TCSANOW, &newattr);
		char c = getchar();
		tcsetattr(0, TCSANOW, &oldattr);
#else
		char c = getch();
#endif
		return Symbol(CNumber(static_cast<long_int_t>(c)));
	}
} // namespace libstd