# NFS Carbon PC - Custom Visual Treatment

A decompiled and enhanced Visual Treatment and screen effects shader for NFS Carbon.

This repo contains reverse engineered shaders with code based mostly on NFS Pro Street and Undercover.

You can use this as a point of reference to make your own shaders in this game or port them from ReShade.

Note that the differences are very subtle as this was primarily made to decompile the shader.

## Enhancements

The enhancements include:

- proper bloom shader for the PC version

- slightly optimized code (such as unrolled loops)

- ported FakeHDR from ReShade directly to game code

- fixed a bug with the GaussianBlur5x5 - made the game call it from the HDR shader instead of ScreenEffect (this is the sole reason why the asi plugin exists)

## Installation

- If not installed, install VS2022 redistributable

- If not installed, install the [NFS Carbon Shader Compiler & Loader](https://github.com/xan1242/xnfs-shaderloader-carbon) or [NFS Carbon PC - Streak Flare fixer](https://github.com/xan1242/nfsc_fixstreakflares)

- Extract files to the game folder and launch

- You can adjust some parameters in fx/visualtreatment.fx such as the size of the vignette and FakeHDR (open and edit it in a text editor)

## Known issues

- uvesovercliff shader is NOT properly ported, but hacked together to look somewhat similar to the original

- Slight shimmering in very dark areas (you need to pixel peep in order to see it) - caused by the ScreenEffects shader, no idea exactly why

- PC version render code is a bit of a mess - the HDR redirect patch is needed to avoid loud bloom in cutscenes



I'm not sure if this repo will *stay* as a purely VT shader repo. If I see it fit, I might evolve this into a general-purpose shader repo for NFS games by Black Box.


