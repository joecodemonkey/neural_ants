# TODO

## Tech Debt

### Decoupling

1. Decouple Texture and Ant so that Ant may be unit tested without Raylib opening a window.
2. Decouple Textures and World so that world may be unit tested without Raylib opening a window
3. Decouple Raylib and game update so that the core game functionality may be tested without Raylib opening a window

### Unit Tests
1. Unit test Surroundings Encoding
2. Unit test Brain
3. Unit test Genome Breeding
4. Ant Initailization
5. Ant Life Cycle


### Simplification
1. Get a Random Vector2 written to simplify testing, maximize reuse


## Potential Bugs
### Food
1. Food Count should never be zero.

## Features
### Food
1. When two ants arrive at food at the same time, they should consume the food equally.

### Analytics
1. Draw a graph of the population fitness over time.
2. When an ant is clicked on in game mode, open a window that shows their brain.
3. In the Ant's brain window, allow the user to toggle seeing what the ant is seeing.
4. Show a 'score' in terms of population 

### Control
1. Allow the user to modify the time step so that time may be accelerated.
2. Allow the user to modify the ant's starting energy level.
3. Allow the user to modify the number of layers in the ant's neural network.
4. Allow the user to modify the neuron count per layer.

### Styling
1. Make Save Game UI look better

### Species
