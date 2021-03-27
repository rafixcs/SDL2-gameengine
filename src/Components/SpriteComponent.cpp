#include "./SpriteComponent.h" 

SpriteComponent::SpriteComponent(const char* filePath) {

}

/*SpriteComponent::~SpriteComponent() {

}*/

void SpriteComponent::SetTexture(std::string assetTextureId) {
    texture = Game::assetManager->GetTexture(assetTextureId);
}

void SpriteComponent::Initialize() {
    transform = owner->GetComponent<TransformComponent>();
    this->srcRect.x = 0;
    this->srcRect.y = 0;
    this->srcRect.w = transform->width;
    this->srcRect.h = transform->height;
}

void SpriteComponent::Update(float deltaTime) {
    this->dstRect.x = static_cast<int>(transform->position.x);
    this->dstRect.y = static_cast<int>(transform->position.y);
    this->dstRect.w = transform->width * transform->scale;
    this->dstRect.h = transform->height * transform->scale;
}

void SpriteComponent::Render() {
    TextureManager::Draw(this->texture, this->srcRect, this->dstRect, spriteFlip);
}
