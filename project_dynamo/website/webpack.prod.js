const path = require("path");
const { merge } = require("webpack-merge");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const TerserPlugin = require("terser-webpack-plugin");

const common = require("./webpack.common");

module.exports = merge(common, {
    mode: "production",
    output: {
        filename: "[name].[hash:8].bundle.js", //Damit verhindert man hashing. Nützlich bei externen libraries
        path: path.resolve(__dirname, "../data"),
    },
    plugins: [
        new MiniCssExtractPlugin({ filename: "[name].[hash:8].css" }) //Slice because SPIFFS doesn't like long filenames
    ],
    module: {
        rules: [
            {
                test: /\.css$/,
                use: [MiniCssExtractPlugin.loader, "css-loader"],
            },
        ],
    },
    optimization: {
        minimize: true,
        minimizer: [
            new TerserPlugin({
                parallel: true,
            }),
        ],
    },
});
