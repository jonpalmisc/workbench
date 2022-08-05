/*
 * Copyright (c) 2022 Jon Palmisciano. All rights reserved.
 *
 * Use of this source code is governed by the BSD 3-Clause license; the full
 * terms of the license can be found in the LICENSE.txt file.
 */

#include "engine.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
	ELEngine* engine = ELCreateEngine(argc, argv);
	if (engine == NULL)
		return 1;

	ELStringResult result = ELEval(engine, "Integrate[3 Cos[x], x]");
	printf("%s\n", result);

	ELFreeResult(engine, result);
	ELDestroyEngine(engine);
}
