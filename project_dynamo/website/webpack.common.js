const path = require("path");
const HtmlWebpackPlugin = require("html-webpack-plugin");

module.exports = {
    entry: {
        main: "./src/index.js"
    },
    plugins: [
        new HtmlWebpackPlugin({
            template: "./src/index.html", // Damit gibt man an, dass die HTML dynamisch erzeugt werden soll mit eben jenem Template. Das brauchen wir weil das JS-File dynamisch reinkommen soll
            inject: "body",
            scriptLoading: "blocking",
        }),
    ],
    module: {
        rules: [
            {
                test: /\.html$/,
                loader: "html-loader",
                options: {
                    esModule: false,
                },
            },
            {
                test: /\.(svg|png|jpe?g|gif)$/,
                use: {
                    loader: "file-loader",
                    options: {
                        name: "[name].[hash].[ext]",
                        outputPath: "images",
                    },
                },
            },
            {
                test: /\.(eot|eot?#iefix|woff2|woff|ttf|svg#Alegreya)$/,
                use: {
                    loader: "file-loader",
                    options: {
                        name: "[name].[ext]",
                        outputPath: "fonts",
                    },
                },
            },
        ],
    },
};
