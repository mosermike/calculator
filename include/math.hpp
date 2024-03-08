/**
 * @author Mike Moser
 * 
 * @file math.hpp
 * @note Enthält verschiedene mathematische Funktionen
 * 
*/


#pragma once

#if __GNUC__
#if __x86_64__ || __ppc64__
#define __ENVIRONMENT64__
#else
#define __ENVIRONMENT32__
#endif
#endif

#ifndef __MATH_HPP_CALC__
#define __MATH_HPP_CALC__

#include "definitions.hpp"
#include "file.hpp"
#include "help.hpp"
#include "log.hpp"
#include "shell.hpp"
#include "standards.hpp"
#include "Timer.hpp"
#include "Unix.hpp"
#include "vector.hpp"
#include "basic_operations.hpp"
#include "derivate.hpp"
#include "functions.hpp"
#include "logging.hpp"
#include "matrix.hpp"



#endif
