/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "utils-tests.hpp"
#include "walkController-tests.hpp"
#include "autoController-tests.hpp"
#include "metaController-tests.hpp"
#include "momentaryActionsMap-tests.hpp"
#include "persistentActionMap-tests.hpp"
#include "lua-tests.hpp"
#include "serialize-tests.hpp"
