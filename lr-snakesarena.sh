#!/usr/bin/env bash

# This file is part of The RetroPie Project
#
# The RetroPie Project is the legal property of its developers, whose names are
# too numerous to list here. Please refer to the COPYRIGHT.md file distributed with this source.
#
# See the LICENSE.md file at the top-level directory of this distribution and
# at https://raw.githubusercontent.com/RetroPie/RetroPie-Setup/master/LICENSE.md
#

rp_module_id="lr-snakesarena"
rp_module_desc="Snakes Arena - Multiplayer Snake Game"
rp_module_help="https://github.com/IgniparousTempest/libretro-snakesarena/wiki"
rp_module_licence="GPL3 https://raw.githubusercontent.com/IgniparousTempest/libretro-snakesarena/master/LICENSE"
rp_module_section="exp"

function depends_lr-snakesarena() {
    getDepends cmake
}

function sources_lr-snakesarena() {
    gitPullOrClone "$md_build" https://github.com/IgniparousTempest/libretro-snakesarena.git
}

function build_lr-snakesarena() {
    cmake .
    make
    md_ret_require="$md_build/snakesarena_libretro.so"
}

function install_lr-snakesarena() {
    md_ret_files=(
        'snakesarena_libretro.so'
        'levels'
        'resources'
        'LICENSE'
        'README.md'
    )
}

function configure_lr-snakesarena() {
    setConfigRoot "ports"

    addPort "$md_id" "snakesarena" "Snakes Arena" "$md_inst/snakesarena_libretro.so"

    ensureSystemretroconfig "ports/snakesarena"
}