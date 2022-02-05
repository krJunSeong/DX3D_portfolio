#pragma once

class Model : public Transform
{
private:
    ModelReader* reader;

    WorldBuffer* worldBuffer;
public:
    Model(string name);
    ~Model();

    void Render();

    ModelReader* GetReader() { return reader; }
};