{
    "targets": [
        {
            "target_name": "anitomy",
            "sources": [
                "anitomy_wrapper.cpp"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "./lib/anitomy"
            ]
        }
    ]
}