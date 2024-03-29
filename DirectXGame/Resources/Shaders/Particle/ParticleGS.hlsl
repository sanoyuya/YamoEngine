#include"Particle.hlsli"

//四角形の頂点数
static const uint vnum = 4;

//センターからのオフセット
static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0), //左下
    float4(-0.5f, +0.5f, 0, 0), //左上
    float4(+0.5f, -0.5f, 0, 0), //右下
    float4(+0.5f, +0.5f, 0, 0) //右上
};

static const float2 uv_array[vnum] =
{
    float2(0, 1), //左下
    float2(0, 0), //左上
    float2(1, 1), //右下
    float2(1, 0) //右上
};

//点の入力から、四角形を出力する
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	//点ストリーム
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element; //出力用頂点データ
    //4点分まわす
    for (uint i = 0; i < vnum; i++)
    {
        //中心からのオフセットをスケーリング
        float4 offset = offset_array[i] * input[0].scale;
        //中心からのオフセットをビルボード回転
        offset = mul(matBillboard, offset);
        
        matrix m = mat;
        
        //Z軸回転
        m[0][0] = cos(input[0].rotation);
        m[0][1] = sin(input[0].rotation);

        m[1][0] = -sin(input[0].rotation);
        m[1][1] = cos(input[0].rotation);
        
        //ワールド座標ベースでずらす
        element.svpos = input[0].pos + offset;
        //ビュー、射影変換
        element.svpos = mul(mat, element.svpos);
        element.uv = uv_array[i];
        element.color = input[0].color;
        output.Append(element);
    }
}