/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of libflo.
 *
 * libflo is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * libflo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libflo.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <libflo/flo.h++>
#include <libflo/node.h++>
#include <libflo/operation.h++>
#include <libflo/version.h++>
#include <libflo/sizet_printf.h++>
#include <string.h>
#include "version.h"
using namespace libflo;

int main(int argc, const char **argv)
{
    /* Prints a simple help text if there isn't a sane set of
     * command-line arguments, which in our case means just a single
     * argument. */
    if (argc <= 1 || argc > 2 || (strcmp(argv[1], "--help") == 0)) {
        fprintf(stderr, "%s <flo>: print statistics for a Flo file\n", argv[0]);
        exit((argc == 2) ? 0 : 1);
    }

    /* If "--version" was passed then print out the version of this
     * program along with the version of libflo that this was linked
     * against. */
    if (strcmp(argv[1], "--version") == 0) {
        fprintf(stderr, "%s %s (using libflo %s)\n",
                argv[0], PCONFIGURE_VERSION, libflo::version());
        exit(0);
    }

    auto flof = flo<node, operation<node> >::parse(argv[1]);

    /* Compute the depth of the circuit, which is the critical
     * path. */
    size_t depth = 0;

    for (const auto& op: flof->operations())
        if (op->dfdepth() > depth)
            depth = op->dfdepth();

    printf("Depth: " SIZET_FORMAT "\n", depth + 1);

    /* Compute the width of the circuit, which is the largest number
     * of nodes at a particular depth. */
    std::vector<size_t> width_hist(depth + 1, 0);

    for (const auto& op: flof->operations())
        width_hist[op->dfdepth()]++;

    size_t width = 0;
    for (const auto& test_width: width_hist)
        if (test_width > width)
            width = test_width;

    printf("Width: " SIZET_FORMAT "\n", width);
}
