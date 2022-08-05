/*
 * Copyright (c) 2022 Jon Palmisciano. All rights reserved.
 *
 * Use of this source code is governed by the BSD 3-Clause license; the full
 * terms of the license can be found in the LICENSE.txt file.
 */

#include <wstp.h>

#include <stdlib.h>

/// A wrapper around a Wolfram Kernel link and environment.
typedef struct {
	WSENV env;
	WSLINK link;
	int lastResultSize;
} ELEngine;

/// A string returned from `ELEval`.
typedef const unsigned char* ELStringResult;

/// Create a new ELEngine with an appropriately-configured Wolfram Kernel link.
///
/// \param argc The number of arguments provided
/// \param argv The arguments to use
ELEngine* ELCreateEngine(int argc, char* argv[])
{
	int error = 0;
	ELEngine* engine = malloc(sizeof(ELEngine));

	// Attempt to initialize the environment.
	WSENV env = WSInitialize((WSEnvironmentParameter)NULL);
	if (env == NULL)
		goto CLEANUP;

	// Attempt to open a link.
	WSLINK link = WSOpenArgcArgv(env, argc, argv, &error);
	if (link == NULL)
		goto CLEANUP;

	engine->env = env;
	engine->link = link;

	return engine;

CLEANUP:
	free(engine);
	return NULL;
}

/// Destroy an `ELEngine`, shutting down the underling Wolfram Kernel link.
///
/// \param engine The engine to shut down
void ELDestroyEngine(ELEngine* engine)
{
	WSClose(engine->link);
	WSDeinitialize(engine->env);
}

/// Evaluate an input string under the given `ELEngine` context. After the
/// result is no longer needed, `ELFreeResult` should be called.
///
/// \param engine The engine to evaluate the input under
/// \param input The input string to evaluate
ELStringResult ELEval(ELEngine* engine, const char* input)
{
	int chars;
	const unsigned char* result;

	WSPutFunction(engine->link, "EvaluatePacket", 1);
	WSPutFunction(engine->link, "ToString", 1);
	WSPutFunction(engine->link, "ToExpression", 1);
	WSPutString(engine->link, input);
	WSEndPacket(engine->link);

	// Wait for return packet.
	while (WSNextPacket(engine->link) != RETURNPKT)
		WSNewPacket(engine->link);

	WSGetUTF8String(engine->link, &result, &engine->lastResultSize, &chars);

	return result;
}

/// Free a result returned by the engine.
///
/// \param engine The engine that previously returned this result
/// \param input The result string to free
void ELFreeResult(ELEngine* engine, ELStringResult result)
{
	WSReleaseUTF8String(engine->link, result, engine->lastResultSize);
}
