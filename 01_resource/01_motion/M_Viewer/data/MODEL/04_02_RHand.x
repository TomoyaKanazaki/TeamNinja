xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 12;
 -1.58951;5.75881;0.96008;,
 1.96662;5.51486;2.48403;,
 1.96662;5.51486;-2.68880;,
 -1.58951;5.75881;-1.16485;,
 -1.88941;3.79336;0.96008;,
 4.29145;0.08546;1.23508;,
 4.29145;0.08546;-1.43985;,
 -1.88941;3.79336;-1.16485;,
 -1.88941;3.79336;-1.16485;,
 4.29145;0.08546;-1.43985;,
 4.29145;0.08546;1.23508;,
 -1.88941;3.79336;0.96008;;
 
 5;
 4;0,1,2,3;,
 4;4,5,1,0;,
 4;3,2,6,7;,
 4;8,9,10,11;,
 4;5,6,2,1;;
 
 MeshMaterialList {
  1;
  5;
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MODEL\\samurai_Tex_New.png";
   }
  }
 }
 MeshNormals {
  5;
  -0.314730;-0.127653;-0.940558;,
  0.068439;0.997655;0.000000;,
  -0.314730;-0.127653;0.940558;,
  -0.514433;-0.857531;0.000000;,
  0.919271;0.393625;0.000000;;
  5;
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;4,4,4,4;;
 }
 MeshTextureCoords {
  12;
  0.896560;0.203030;,
  0.868010;0.193170;,
  0.872210;0.175810;,
  0.909800;0.180870;,
  0.885910;0.225190;,
  0.825150;0.190730;,
  0.825480;0.160870;,
  0.909800;0.149410;,
  0.442610;0.291990;,
  0.498880;0.289850;,
  0.498880;0.310730;,
  0.442610;0.308580;;
 }
}
