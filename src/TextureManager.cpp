bool TextureManager::initialize()
{
    TextureGL logo;

    if (!logo.load("assets/images/logo.png"))
        return false;

    m_textures.push_back(std::move(logo));

    return true;
}