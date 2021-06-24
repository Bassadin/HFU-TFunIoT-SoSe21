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
    module: {
        rules: [
            {
                test: /\.scss$/,
                use: ["style-loader", 'css-loader', 'sass-loader'],
            },
        ],
    },
});
