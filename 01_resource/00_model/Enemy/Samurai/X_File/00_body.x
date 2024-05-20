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
 18;
 -13.06644;14.18462;6.98712;,
 -11.18220;26.11194;3.98929;,
 -10.48760;25.87788;-3.75021;,
 -12.81001;14.17647;-4.61520;,
 0.00000;14.67966;9.64373;,
 0.00000;27.18331;6.94633;,
 -0.00000;24.88585;-9.48100;,
 -0.00000;13.11086;-10.74239;,
 13.06644;14.18462;6.98712;,
 12.81001;14.17647;-4.61520;,
 10.48761;25.87788;-3.75021;,
 11.18221;26.11194;3.98928;,
 -21.50298;0.94746;-7.52033;,
 -0.00000;-0.70468;-16.98967;,
 -21.38000;0.70151;13.96440;,
 0.00000;0.77190;17.53077;,
 21.38000;0.70151;13.96440;,
 21.50298;0.94746;-7.52034;;
 
 12;
 4;0,1,2,3;,
 4;4,5,1,0;,
 4;3,2,6,7;,
 4;8,9,10,11;,
 4;4,8,11,5;,
 4;9,7,6,10;,
 4;12,3,7,13;,
 4;14,0,3,12;,
 4;15,4,0,14;,
 4;15,16,8,4;,
 4;16,17,9,8;,
 4;17,13,7,9;;
 
 MeshMaterialList {
  1;
  12;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "C:\\Users\\student\\Desktop\\フォルダ集\\MyFolder\\MetasequiaLE専用\\2025\\Mqp\\data\\TEXTURE\\MODEL\\samurai_Tex_New.png";
   }
  }
 }
 MeshNormals {
  24;
  -0.982741;0.175717;-0.057824;,
  -0.841314;0.540532;-0.003951;,
  -0.820687;0.173966;-0.544251;,
  -0.687735;0.537648;-0.487806;,
  0.982741;0.175717;-0.057825;,
  0.167020;0.503610;0.847633;,
  0.820687;0.173965;-0.544251;,
  0.687735;0.537648;-0.487806;,
  0.000000;0.510785;0.859708;,
  -0.000000;0.141074;-0.989999;,
  -0.000000;0.424143;-0.905595;,
  0.000000;0.242950;0.970039;,
  -0.000000;0.288677;-0.957427;,
  -0.768975;0.363095;-0.526156;,
  -0.930349;0.365318;-0.031508;,
  0.000000;0.381854;0.924223;,
  0.930349;0.365318;-0.031509;,
  0.768975;0.363094;-0.526156;,
  -0.235331;0.236126;0.942796;,
  -0.203355;0.373875;0.904911;,
  0.203355;0.373875;0.904911;,
  0.235332;0.236127;0.942796;,
  -0.167020;0.503610;0.847633;,
  0.841314;0.540532;-0.003952;;
  12;
  4;14,0,2,13;,
  4;15,11,18,19;,
  4;13,2,9,12;,
  4;16,17,6,4;,
  4;15,20,21,11;,
  4;17,12,9,6;,
  4;3,13,12,10;,
  4;1,14,13,3;,
  4;8,15,19,22;,
  4;8,5,20,15;,
  4;23,7,17,16;,
  4;7,10,12,17;;
 }
 MeshTextureCoords {
  18;
  0.229881;0.377804;,
  0.272512;0.281407;,
  0.234151;0.237630;,
  0.151960;0.293494;,
  0.362919;0.381054;,
  0.362919;0.272785;,
  0.212857;0.150665;,
  0.113602;0.160645;,
  0.229881;0.377804;,
  0.151960;0.293494;,
  0.234151;0.237630;,
  0.272512;0.281407;,
  0.058108;0.357283;,
  0.000263;0.172042;,
  0.181202;0.487878;,
  0.362919;0.504685;,
  0.181202;0.487878;,
  0.058108;0.357283;;
 }
}
