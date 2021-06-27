const path = require("path");
const { merge } = require("webpack-merge");

const common = require("./webpack.common");

module.exports = merge(common, {
    mode: "development",
    devtool: 'inline-source-map',
    output: {
        filename: "[name].bundle.js",
        path: path.resolve(__dirname, "dist"),
    },
    devServer: {
        host: '0.0.0.0',
        disableHostCheck: true,
        useLocalIp: true,
    },
    module: {
        rules: [
            {
                test: /\.scss$/,
                use: ["style-loader", 'css-loader', 'sass-loader'],
            },
        ],
    },
});
