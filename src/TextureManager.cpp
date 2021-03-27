#include "./TextureManager.h" 
#include <iostream>

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    //std::cout << "File name" << fileName << std::endl;
    
    SDL_Surface* surface = IMG_Load(fileName);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    
    if (texture == nullptr) {
        std::cerr << "Failed to load Texture: " << IMG_GetError() << std::endl;
    }

    SDL_FreeSurface(surface);
    return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect dstRect, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::renderer, texture, &srcRect, &dstRect, 0.0, NULL, flip);
}