{
    "targets": [
        {
            "target_name": "anitomy",
            "sources": [
                "anitomy.cpp"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "./lib/anitomy"
            ]
        }
    ]
}