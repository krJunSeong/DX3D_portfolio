#include "framework.h"

Horse::Horse()
    :Model("horse")
{
    Load();
}

Horse::~Horse()
{
}

void Horse::Update()
{
    UpdateWorld();

}

void Horse::Render()
{
    Model::Render();
}
